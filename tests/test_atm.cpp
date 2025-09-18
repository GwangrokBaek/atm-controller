#include <iostream>

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

int main()
{
    cout << "Tests passed : " << passed << ", failed : " << failed << endl;
}