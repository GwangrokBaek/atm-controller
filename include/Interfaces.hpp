#pragma once
#include <string>
#include "Result.hpp"

using namespace std;

using Card = string;

class ICardReader{
public:
    virtual ~ICardReader() = default;

    virtual Result<Card> read(void) = 0;
    virtual Status eject(void) = 0;
};