#pragma once
#include "Interfaces.hpp"

class FakeCardReader : public ICardReader {
public:
    Card& card;
    bool inserted = false;
    bool ejected = false;

    FakeCardReader(Card& card) : card(card)
    {}

    Card& read(void)
    {
        inserted = true;
        return card;
    }

    void eject(void)
    {
        ejected = true;
        return;
    }
};