#pragma once
#include "Interfaces.hpp"
#include <optional>

using namespace std;

class Controller {
public:
    enum class State { Idle, CardInserted, Authenticated, AccountSelected };

    struct Config {
        int maxPinAttempts = 3;
    };

private:
    State _state = State::Idle;
    ICardReader& _cardReader;
    IBank& _bank;

    Config _cfg;

    optional<Card> _card;
    optional<AccountId> _account;

    int _pinAttempts;

    static bool validMoney(int money)
    {
        return money >= 0;
    }

    // TODO: Cash Bin

public:
    Controller(ICardReader& cardReader, IBank& bank) : _cardReader(cardReader), _bank(bank)
    {}

    State state(void) const;
    
    // Card
    Status insertCard(void);
    Status ejectCard(void);

    // Bank
    Status enterPin(const Pin& pin);
    Result<vector<AccountId>> listAccounts() const;
    Status selectAccount(const AccountId& accountId);
    Result<int> getBalance(void) const;
    Status deposit(int money);
    Status withdraw(int money);

    // TODO: Cash Bin
};