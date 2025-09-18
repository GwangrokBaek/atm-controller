#pragma once
#include <string>

using namespace std;

struct Card {
    string cardId;
};

class ICardReader{
public:
    virtual ~ICardReader() = default;

    virtual void insertCard(const Card& card) = 0;
    virtual void ejectCard() = 0;
};