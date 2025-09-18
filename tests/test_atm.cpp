#include <iostream>
#include "Controller.hpp"
#include "FakeCardReader.hpp"

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
    Card card{"12345", true};
    FakeCardReader cardReader(card);
    Controller atm(cardReader);

    REQUIRE(atm.insertCard());
    REQUIRE(cardReader.card.cardId == "12345");
    REQUIRE(cardReader.inserted == true);
    REQUIRE(atm.state() == Controller::State::CardInserted);
    REQUIRE(atm.ejectCard());
    REQUIRE(cardReader.ejected == true);
    REQUIRE(atm.state() == Controller::State::Idle);
END_TEST

int main()
{
    test_card_insert_and_eject();

    cout << "Tests passed : " << passed << ", failed : " << failed << endl;
}