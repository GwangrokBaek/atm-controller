#pragma once
#include "Interfaces.hpp"

class FakeCardReader : public ICardReader {
public:
    Result<Card> card;
    bool inserted = false;
    bool ejected = false;

    FakeCardReader(Card& card) : card(card)
    {}

    Result<Card> read(void)
    {
        inserted = true;
        return card;
    }

    Status eject(void)
    {
        ejected = true;
        return Status::okStatus();
    }
};