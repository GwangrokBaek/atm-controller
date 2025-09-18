#include "Controller.hpp"

Controller::State Controller::state(void) const
{
    return _state;
};

// Card
Status Controller::insertCard(void)
{
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

Status Controller::ejectCard(void)
{
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

// Bank
Status Controller::enterPin(const Pin& pin)
{
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

Result<vector<AccountId>> Controller::listAccounts() const
{
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

Status Controller::selectAccount(const AccountId& accountId)
{
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

Result<int> Controller::getBalance(void) const
{
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

Status Controller::deposit(int money)
{
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

Status Controller::withdraw(int money)
{
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

    // TODO: check cash bin capacity

    auto status = _bank.withdraw(*_account, money);
    if (!status.isOk())
    {
        return status;
    }

    // TODO: check if it is really withdrawed by cash bin

    return Status::okStatus();
}