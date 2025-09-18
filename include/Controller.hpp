#pragma once
#include "Interfaces.hpp"
#include <optional>

using namespace std;

/**
 * @brief Central controller for ATM operations
 */
class Controller {
public:
    /**
     * @brief ATM operational states
     */
    enum class State {
        Idle,            ///< No active session, waiting for card
        CardInserted,    ///< Card present, awaiting PIN
        Authenticated,   ///< PIN verified, awaiting account selection
        AccountSelected  ///< Account selected, ready for transactions
    };

    /**
     * @brief Configuration parameters for ATM behavior
     */
    struct Config {
        int maxPinAttempts = 3;  ///< Maximum failed PIN attempts before card ejection
    };

private:
    State _state = State::Idle;
    
    ICardReader& _cardReader;            // Card reader interface
    IBank& _bank;                        // Banking service interface
    ICashBin& _cashBin;                  // Cash bin interface

    Config _cfg;                         // ATM configuration

    optional<Card> _card;                // Currently inserted card
    optional<AccountId> _account;        // Currently selected account
    
    int _pinAttempts;                    // Failed PIN attempts

    /**
     * @brief Validate amount
     * 
     * @param money Amount to validate
     */
    static bool validMoney(int money)
    {
        return money >= 0;
    }

public:
    /**
     * @brief Construct ATM Controller with required interfaces
     * 
     * @param cardReader Hardware interface for card operations
     * @param bank Service interface for banking operations
     * @param cashBin Hardware interface for cash dispensing
     */
    Controller(ICardReader& cardReader, IBank& bank, ICashBin& cashBin)
     : _cardReader(cardReader), _bank(bank), _cashBin(cashBin)
    {}

    /**
     * @brief Get current ATM state
     */
    State state(void) const;
    
    /**
     * @brief Insert and read a card
     */
    Status insertCard(void);
    
    /**
     * @brief Eject the current card and end session
     */
    Status ejectCard(void);
    
    /**
     * @brief Enter PIN for authentication
     * 
     * @param pin The PIN code to verify
     */
    Status enterPin(const Pin& pin);
    
    /**
     * @brief List all accounts associated with the current card
     */
    Result<vector<AccountId>> listAccounts() const;
    
    /**
     * @brief Select an account for transactions
     * 
     * @param accountId The account to select
     */
    Status selectAccount(const AccountId& accountId);
    
    /**
     * @brief Get balance of currently selected account
     */
    Result<int> getBalance(void) const;
    
    /**
     * @brief Deposit money into currently selected account
     * 
     * @param money Amount to deposit
     */
    Status deposit(int money);
    
    /**
     * @brief Withdraw money from currently selected account
     * 
     * @param money Amount to withdraw
     */
    Status withdraw(int money);
};
