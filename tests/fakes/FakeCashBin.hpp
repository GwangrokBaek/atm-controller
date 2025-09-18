#pragma once
#include "Interfaces.hpp"

class FakeCashBin : public ICashBin {
public:
    int capacity;

    FakeCashBin(int capacity) : capacity(capacity)
    {}

    Status canDispense(int money)
    {
        return (money <= capacity) ? Status::okStatus() : Status::error(Err::InsufficientCashBin);
    }

    Status dispense(int money)
    {
        if (money <= capacity)
        {
            capacity -= money;
            return Status::okStatus();
        }
        return Status::error(Err::InsufficientCashBin);
    }
    
    int getCurrentCapacity() const
    {
        return capacity;
    }
};
