#pragma once
#include <string>
#include <vector>
#include "Result.hpp"

using namespace std;

using Card = string;
using Pin = string;
using AccountId = string;

class IBank {
public:
    virtual ~IBank() = default;

    virtual Status verifyPin(const Card& card, const Pin& pin) = 0;
    virtual vector<AccountId> listAccounts(const Card& card) = 0;
    virtual Result<int> getBalance(const AccountId& accountId) = 0;
    
    virtual Status deposit(const AccountId& accountId, int money) = 0;
    virtual Status canWithdraw(const AccountId& accountId, int money) = 0;
    virtual Status withdraw(const AccountId& AccountId, int money) = 0;
};

class ICardReader {
public:
    virtual ~ICardReader() = default;

    virtual Result<Card> read(void) = 0;
    virtual Status eject(void) = 0;
};