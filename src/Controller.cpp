#include "Controller.hpp"

Controller::State Controller::state(void) const
{
    return _state;
};

bool Controller::insertCard(void)
{
    if (_state != State::Idle)
    {
        return false;
    }

    _card = _cardReader.read();
    if (_card->success == false)
    {
        return false;
    }

    _pinAttempts = 0;
    _state = State::CardInserted;

    return true;
}

bool Controller::ejectCard(void)
{
    if (_state == State::Idle)
    {
        return false;
    }

    _cardReader.eject();
    _card.reset();
    // TODO: reset account info

    _pinAttempts = 0;
    _state = State::Idle;

    return true;
}