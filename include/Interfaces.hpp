#pragma once
#include <string>

using namespace std;

// TODO: New Result struct with template
struct Card {
    string cardId;
    bool success;
};

class ICardReader{
public:
    virtual ~ICardReader() = default;

    virtual Card& read(void) = 0;
    virtual void eject(void) = 0;
};