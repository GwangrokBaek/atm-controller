#include "test_framework.hpp"
#include "Controller.hpp"
#include "fakes/FakeCardReader.hpp"
#include "fakes/FakeBank.hpp"
#include "fakes/FakeCashBin.hpp"
#include <unordered_map>
#include <vector>

using namespace std;

/**
 * @brief Test withdrawal and balance checking functionality
 * 
 * - Successful PIN entry and authentication
 * - Account selection
 * - Balance retrieval
 * - Successful withdrawal
 * - Balance update after withdrawal
 */
TEST(test_withdraw_and_balance)
    Card card = "CARD-001";
    Pin pin = "12345";
    AccountId account1 = "ACCOUNT-001";
    AccountId account2 = "ACCOUNT-002";
    int balance1 = 1000;
    int balance2 = 1200;
    int capacity = 10000;

    unordered_map<Card, Pin> pinMap = {{card, pin}};
    unordered_map<Card, vector<AccountId>> accountsMap = {{card, {account1, account2}}};
    unordered_map<AccountId, int> balanceMap = {{account1, balance1}, {account2, balance2}};

    FakeBank bank(pinMap, accountsMap, balanceMap);
    FakeCashBin cashBin(capacity);
    FakeCardReader cardReader(card);
    Controller atm(cardReader, bank, cashBin);

    // Complete authentication flow
    REQUIRE(atm.insertCard().isOk());
    REQUIRE(atm.enterPin(pin).isOk());
    REQUIRE(atm.selectAccount(account1).isOk());
    
    // Test balance retrieval
    auto b1 = atm.getBalance();
    REQUIRE(b1.isOk());
    REQUIRE(b1.value() == balance1);
    
    // Test withdrawal
    REQUIRE(atm.withdraw(120).isOk());
    
    // Verify balance updated correctly
    auto b2 = atm.getBalance();
    REQUIRE(b2.isOk());
    REQUIRE(b2.value() == balance1 - 120);
    
    // Clean up
    REQUIRE(atm.ejectCard().isOk());
    REQUIRE(atm.state() == Controller::State::Idle);
END_TEST

/**
 * @brief Test deposit and balance checking functionality
 * 
 * - Successful deposit operation
 * - Balance increase after deposit
 */
TEST(test_deposit_and_balance)
    Card card = "CARD-001";
    Pin pin = "12345";
    AccountId account1 = "ACCOUNT-001";
    AccountId account2 = "ACCOUNT-002";
    int balance1 = 1000;
    int balance2 = 1200;
    int capacity = 10000;

    unordered_map<Card, Pin> pinMap = {{card, pin}};
    unordered_map<Card, vector<AccountId>> accountsMap = {{card, {account1, account2}}};
    unordered_map<AccountId, int> balanceMap = {{account1, balance1}, {account2, balance2}};

    FakeBank bank(pinMap, accountsMap, balanceMap);
    FakeCashBin cashBin(capacity);
    FakeCardReader cardReader(card);
    Controller atm(cardReader, bank, cashBin);

    // Complete authentication flow
    REQUIRE(atm.insertCard().isOk());
    REQUIRE(atm.enterPin(pin).isOk());
    REQUIRE(atm.selectAccount(account1).isOk());
    
    // Test balance retrieval before deposit
    auto b1 = atm.getBalance();
    REQUIRE(b1.isOk());
    REQUIRE(b1.value() == balance1);
    
    // Test deposit
    REQUIRE(atm.deposit(250).isOk());
    
    // Verify balance updated correctly
    auto b2 = atm.getBalance();
    REQUIRE(b2.isOk());
    REQUIRE(b2.value() == balance1 + 250);
    
    // Clean up
    REQUIRE(atm.ejectCard().isOk());
    REQUIRE(atm.state() == Controller::State::Idle);
END_TEST

/**
 * @brief Test PIN authentication failure scenarios
 * 
 * - Wrong PIN is rejected
 * - Multiple wrong PIN attempts lead to card ejection
 */
TEST(test_pin_authentication)
    Card card = "CARD-001";
    Pin correctPin = "12345";
    Pin wrongPin = "99999";
    AccountId account1 = "ACCOUNT-001";
    int balance1 = 1000;

    unordered_map<Card, Pin> pinMap = {{card, correctPin}};
    unordered_map<Card, vector<AccountId>> accountsMap = {{card, {account1}}};
    unordered_map<AccountId, int> balanceMap = {{account1, balance1}};

    FakeBank bank(pinMap, accountsMap, balanceMap);
    FakeCashBin cashBin(1000);
    FakeCardReader cardReader(card);
    Controller atm(cardReader, bank, cashBin);

    // Insert card
    REQUIRE(atm.insertCard().isOk());
    REQUIRE(atm.state() == Controller::State::CardInserted);
    
    // Try wrong PIN
    REQUIRE(!atm.enterPin(wrongPin).isOk());
    REQUIRE(atm.state() == Controller::State::CardInserted);
    
    // Try correct PIN
    REQUIRE(atm.enterPin(correctPin).isOk());
    REQUIRE(atm.state() == Controller::State::Authenticated);
    
    // Clean up
    REQUIRE(atm.ejectCard().isOk());
    REQUIRE(atm.state() == Controller::State::Idle);
END_TEST
