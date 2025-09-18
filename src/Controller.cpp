#include "Controller.hpp"
#include <exception>
#include <stdexcept>

Controller::State Controller::state(void) const
{
    return _state;
};

// Card
Status Controller::insertCard(void)
{
    try {
        if (_state != State::Idle)
        {
            return Status::error(Err::InvalidState);
        }

        auto result = _cardReader.read();
        if (!result.isOk()) return Status::error(result.error());

        _card = result.value();
        _pinAttempts = 0;
        _state = State::CardInserted;

        return Status::okStatus();
    }
    catch (const std::bad_alloc& e) {
        return Status::error(Err::MemoryError);
    }
    catch (const std::runtime_error& e) {
        return Status::error(Err::HardwareError);
    }
    catch (const std::exception& e) {
        return Status::error(Err::SystemError);
    }
    catch (...) {
        return Status::error(Err::SystemError);
    }
}

Status Controller::ejectCard(void)
{
    try {
        if (_state == State::Idle)
        {
            return Status::error(Err::InvalidState);
        }

        _cardReader.eject();
        _card.reset();
        _account.reset();

        _pinAttempts = 0;
        _state = State::Idle;

        return Status::okStatus();
    }
    catch (const std::runtime_error& e) {
        // Reset state anyway to avoid getting stuck
        _card.reset();
        _account.reset();
        _pinAttempts = 0;
        _state = State::Idle;
        return Status::error(Err::HardwareError);
    }
    catch (const std::exception& e) {
        // Reset state anyway to avoid getting stuck
        _card.reset();
        _account.reset();
        _pinAttempts = 0;
        _state = State::Idle;
        return Status::error(Err::SystemError);
    }
    catch (...) {
        // Reset state anyway to avoid getting stuck
        _card.reset();
        _account.reset();
        _pinAttempts = 0;
        _state = State::Idle;
        return Status::error(Err::SystemError);
    }
}

// Bank
Status Controller::enterPin(const Pin& pin)
{
    try {
        if (_state != State::CardInserted)
        {
            return Status::error(Err::InvalidState);
        }

        if (!_card)
        {
            return Status::error(Err::CardAbsent);
        }

        if (!_bank.verifyPin(*_card, pin).isOk())
        {
            ++_pinAttempts;
            if (_pinAttempts >= _cfg.maxPinAttempts)
            {
                Controller::ejectCard();
                return Status::error(Err::PinFailed);
            }
            return Status::error(Err::PinFailed);
        }

        _state = State::Authenticated;

        return Status::okStatus();
    }
    catch (const std::runtime_error& e) {
        // Network or service error during PIN verification
        return Status::error(Err::NetworkError);
    }
    catch (const std::exception& e) {
        return Status::error(Err::SystemError);
    }
    catch (...) {
        return Status::error(Err::SystemError);
    }
}

Result<vector<AccountId>> Controller::listAccounts() const
{
    try {
        if (_state != State::Authenticated)
        {
            return Err::InvalidState;
        }

        if (!_card)
        {
            return Err::CardAbsent;
        }

        return _bank.listAccounts(*_card);
    }
    catch (const std::runtime_error& e) {
        // Network error while fetching accounts
        return Err::NetworkError;
    }
    catch (const std::exception& e) {
        return Err::SystemError;
    }
    catch (...) {
        return Err::SystemError;
    }
}

Status Controller::selectAccount(const AccountId& accountId)
{
    try {
        if (_state != State::Authenticated)
        {
            return Status::error(Err::InvalidState);
        }

        if (!_card)
        {
            return Status::error(Err::CardAbsent);
        }

        auto accounts = _bank.listAccounts(*_card);
        bool found = false;

        for (auto& a : accounts)
        {
            if (a == accountId)
            {
                found = true;
                break;
            }
        }

        if (!found)
        {
            return Status::error(Err::AccountAbsent);
        }

        _account = accountId;
        _state = State::AccountSelected;

        return Status::okStatus();
    }
    catch (const std::runtime_error& e) {
        return Status::error(Err::NetworkError);
    }
    catch (const std::exception& e) {
        return Status::error(Err::SystemError);
    }
    catch (...) {
        return Status::error(Err::SystemError);
    }
}

Result<int> Controller::getBalance(void) const
{
    try {
        if (_state != State::AccountSelected)
        {
            return Err::InvalidState;
        }

        if (!_account)
        {
            return Err::AccountNotSelected;
        }

        return _bank.getBalance(*_account);
    }
    catch (const std::runtime_error& e) {
        return Err::NetworkError;
    }
    catch (const std::exception& e) {
        return Err::SystemError;
    }
    catch (...) {
        return Err::SystemError;
    }
}

Status Controller::deposit(int money)
{
    try {
        if (_state != State::AccountSelected)
        {
            return Status::error(Err::InvalidState);
        }

        if (!_account)
        {
            return Status::error(Err::AccountNotSelected);
        }

        if (!validMoney(money))
        {
            return Status::error(Err::InvalidArg);
        }

        return _bank.deposit(*_account, money);
    }
    catch (const std::runtime_error& e) {
        return Status::error(Err::NetworkError);
    }
    catch (const std::exception& e) {
        return Status::error(Err::SystemError);
    }
    catch (...) {
        return Status::error(Err::SystemError);
    }
}

Status Controller::withdraw(int money)
{
    try {
        if (_state != State::AccountSelected)
        {
            return Status::error(Err::InvalidState);
        }

        if (!_account)
        {
            return Status::error(Err::AccountNotSelected);
        }

        if (!validMoney(money))
        {
            return Status::error(Err::InvalidArg);
        }

        if (!_bank.canWithdraw(*_account, money).isOk())
        {
            return Status::error(Err::InsufficientBank);
        }

        if (!_cashBin.canDispense(money).isOk())
        {
            return Status::error(Err::InsufficientCashBin);
        }

        auto status = _bank.withdraw(*_account, money);
        if (!status.isOk())
        {
            return status;
        }

        status = _cashBin.dispense(money);
        if (!status.isOk())
        {
            // Rollback: deposit money back to account
            try {
                _bank.deposit(*_account, money);
            } catch (...) {
                return Status::error(Err::SystemError);
            }
            return status;
        }

        return Status::okStatus();
    }
    catch (const std::bad_alloc& e) {
        return Status::error(Err::MemoryError);
    }
    catch (const std::runtime_error& e) {
        return Status::error(Err::NetworkError);
    }
    catch (const std::exception& e) {
        return Status::error(Err::SystemError);
    }
    catch (...) {
        return Status::error(Err::SystemError);
    }
}
