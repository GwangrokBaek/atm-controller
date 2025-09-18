#pragma once
#include <string>
#include <vector>
#include "Result.hpp"

using namespace std;

using Card = string;      // bank card id
using Pin = string;       // PIN code
using AccountId = string; // bank account id

/**
 * @brief Interface for bank service operations
 */
class IBank {
public:
    virtual ~IBank() = default;

    /**
     * @brief Verify a PIN code for the given card
     * 
     * @param card The card id to query
     * @param pin The PIN code to check
     */
    virtual Status verifyPin(const Card& card, const Pin& pin) = 0;
    
    /**
     * @brief Retrieve all accounts associated with a card
     * 
     * @param card The card id to query
     */
    virtual vector<AccountId> listAccounts(const Card& card) = 0;
    
    /**
     * @brief Get the current balance of an account
     * 
     * @param accountId The account to check
     */
    virtual Result<int> getBalance(const AccountId& accountId) = 0;
    
    /**
     * @brief Deposit money into an account
     * 
     * @param accountId The account to check
     * @param money The amount to deposit
     */
    virtual Status deposit(const AccountId& accountId, int money) = 0;
    
    /**
     * @brief Check if withdrawal is possible without executing it
     * 
     * @param accountId The account to check
     * @param money The amount to widthdraw
     */
    virtual Status canWithdraw(const AccountId& accountId, int money) = 0;
    
    /**
     * @brief Withdraw money from an account
     * 
     * @param accountId The account to check
     * @param money The amount to withdraw
     */
    virtual Status withdraw(const AccountId& accountId, int money) = 0;
};

/**
 * @brief Interface for card reader hardware operations
 */
class ICardReader {
public:
    virtual ~ICardReader() = default;

    /**
     * @brief Read card data
     */
    virtual Result<Card> read(void) = 0;
    
    /**
     * @brief Eject the card
     */
    virtual Status eject(void) = 0;
};

/**
 * @brief Interface for cash dispensing hardware operations
 */
class ICashBin {
public:
    virtual ~ICashBin() = default;
    
    /**
     * @brief Check if the requested amount can be dispensed
     * 
     * @param money Amount to check
     */
    virtual Status canDispense(int money) = 0;
    
    /**
     * @brief Dispense the requested amount of cash
     * 
     * @param money Amount to dispense
     */
    virtual Status dispense(int money) = 0;
};
