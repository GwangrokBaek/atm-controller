#pragma once
#include "Interfaces.hpp"
#include <optional>

using namespace std;

class Controller {
public:
    enum class State { Idle, CardInserted, Authenticated, AccountSelected };

private:
    State _state = State::Idle;
    ICardReader& _cardReader;

    optional<Card> _card;

    int _pinAttempts;

    // TODO: Bank

    // TODO: Cash Bin

public:
    Controller(ICardReader& cardReader) : _cardReader(cardReader)
    {}

    State state(void) const;
    
    // Card
    bool insertCard(void);
    bool ejectCard(void);

    // TODO: Bank

    // TODO: Cash Bin
};