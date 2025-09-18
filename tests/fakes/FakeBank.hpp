#pragma once
#include <vector>
#include <unordered_map>
#include "Interfaces.hpp"

using namespace std;

class FakeBank : public IBank {
public:
    unordered_map<Card, Pin> pinMap;
    unordered_map<Card, vector<AccountId>> accountsMap;
    unordered_map<AccountId, int> balanceMap;

    FakeBank(unordered_map<Card, Pin> pinMap,
             unordered_map<Card, vector<AccountId>> accountsMap,
             unordered_map<AccountId, int> balanceMap)
             : pinMap(pinMap), accountsMap(accountsMap), balanceMap(balanceMap)
    {}

    Status verifyPin(const Card& card, const Pin& pin)
    {
        auto it = pinMap.find(card);
        if (it != pinMap.end() && it->second == pin) {
            return Status::okStatus();
        }
        
        return Status::error(Err::InvalidArg);
    }

    vector<AccountId> listAccounts(const Card& card)
    {
        auto it = accountsMap.find(card);
        if (it != accountsMap.end()) {
            return it->second;
        }

        return vector<AccountId>();
    }

    Result<int> getBalance(const AccountId& accountId)
    {
        auto it = balanceMap.find(accountId);
        if (it != balanceMap.end()) {
            return Result<int>(it->second);
        }

        return Result<int>(Err::InvalidArg);
    }

    Status deposit(const AccountId& accountId, int money)
    {
        auto it = balanceMap.find(accountId);
        if (it != balanceMap.end()) {
            it->second += money;
            return Status::okStatus();
        }

        return Status::error(Err::InvalidArg);
    }

    Status canWithdraw(const AccountId& accountId, int money)
    {
        auto it = balanceMap.find(accountId);
        if (it != balanceMap.end() && it->second >= money)
        {
            return Status::okStatus();
        }

        return Status::error(Err::InsufficientBank);
    }

    Status withdraw(const AccountId& accountId, int money)
    {
        auto it = balanceMap.find(accountId);
        if (it != balanceMap.end() && it->second >= money) {
            it->second -= money;
            return Status::okStatus();
        }

        return Status::error(Err::InvalidArg);
    }
};