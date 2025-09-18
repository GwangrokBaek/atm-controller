#include <iostream>
#include "Controller.hpp"
#include "FakeCardReader.hpp"
#include "FakeBank.hpp"

using namespace std;

static int passed = 0;
static int failed = 0;

#if defined(_MCS_VER)
#define FUNC_NAME __FUNCTION__
#else
#define FUNC_NAME __func__
#endif

#define TEST(name) \
    void name(); \
    void name() { \
        cout << "[ RUN ] " << FUNC_NAME << endl; \

#define END_TEST \
    cout << "[ OK  ] " << FUNC_NAME << endl; \
    cout << endl; \
    }

#define REQUIRE(cond) do { if(!(cond)) { cerr << "REQUIRE failed: " #cond "\n"; ++failed; } else ++passed; } while(0)

TEST(test_card_insert_and_eject)
    Card card = "CARD-001";

    FakeBank bank({{"", ""}}, {{"", {}}}, {{},{}});
    FakeCardReader cardReader(card);
    Controller atm(cardReader, bank);

    REQUIRE(atm.insertCard().isOk());
    REQUIRE(cardReader.card.value() == card);
    REQUIRE(cardReader.inserted == true);
    REQUIRE(atm.state() == Controller::State::CardInserted);
    REQUIRE(atm.ejectCard().isOk());
    REQUIRE(cardReader.ejected == true);
    REQUIRE(atm.state() == Controller::State::Idle);
END_TEST

TEST(test_withdraw_and_balance)
    Card card = "CARD-001";
    Pin pin = "12345";
    AccountId account1 = "ACCOUNT-001";
    AccountId account2 = "ACCOUNT-002";
    int balance1 = 1000;
    int balance2 = 1200;

    unordered_map<Card, Pin> pinMap = {{card, pin}};
    unordered_map<Card, vector<AccountId>> accountsMap = {{card, {account1, account2}}};
    unordered_map<AccountId, int> balanceMap = {{account1, balance1}, {account2, balance2}};

    FakeBank bank(pinMap, accountsMap, balanceMap);
    FakeCardReader cardReader(card);
    Controller atm(cardReader, bank);

    REQUIRE(atm.insertCard().isOk());
    REQUIRE(atm.enterPin(pin).isOk());
    REQUIRE(atm.selectAccount(account1).isOk());
    auto b1 = atm.getBalance();
    REQUIRE(b1.isOk());
    REQUIRE(atm.withdraw(120).isOk());
    auto b2 = atm.getBalance();
    REQUIRE(b2.isOk());
    REQUIRE(b2.value() == b1.value() - 120);
    REQUIRE(atm.ejectCard().isOk());
    REQUIRE(atm.state() == Controller::State::Idle);
END_TEST

TEST(test_deposit_and_balance)
    Card card = "CARD-001";
    Pin pin = "12345";
    AccountId account1 = "ACCOUNT-001";
    AccountId account2 = "ACCOUNT-002";
    int balance1 = 1000;
    int balance2 = 1200;

    unordered_map<Card, Pin> pinMap = {{card, pin}};
    unordered_map<Card, vector<AccountId>> accountsMap = {{card, {account1, account2}}};
    unordered_map<AccountId, int> balanceMap = {{account1, balance1}, {account2, balance2}};

    FakeBank bank(pinMap, accountsMap, balanceMap);
    FakeCardReader cardReader(card);
    Controller atm(cardReader, bank);

    REQUIRE(atm.insertCard().isOk());
    REQUIRE(atm.enterPin(pin).isOk());
    REQUIRE(atm.selectAccount(account1).isOk());
    auto b1 = atm.getBalance();
    REQUIRE(b1.isOk());
    REQUIRE(atm.deposit(250).isOk());
    auto b2 = atm.getBalance();
    REQUIRE(b2.isOk());
    REQUIRE(b2.value() == b1.value() + 250);
    REQUIRE(atm.ejectCard().isOk());
    REQUIRE(atm.state() == Controller::State::Idle);
END_TEST

int main()
{
    test_card_insert_and_eject();
    test_withdraw_and_balance();
    test_deposit_and_balance();

    cout << "Tests passed : " << passed << ", failed : " << failed << endl;
}