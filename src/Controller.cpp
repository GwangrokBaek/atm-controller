#include "Controller.hpp"

Controller::State Controller::state(void) const
{
    return _state;
};

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
    // TODO: reset account info

    _pinAttempts = 0;
    _state = State::Idle;

    return Status::okStatus();
}