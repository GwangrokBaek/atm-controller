# ATM Test Framework

## 테스트 구조

### 테스트 프레임워크
- `test_framework.hpp` - 테스트 매크로 및 유틸리티 함수 정의
- `test_framework.cpp` - 테스트 프레임워크 구현
- `test_runner.cpp` - 메인 테스트 러너

### 테스트 케이스 모듈
- `card_tests.cpp` - 카드 관련 테스트 케이스
- `banking_tests.cpp` - 은행 관련 테스트 케이스

### Fake 모듈
- `fakes/FakeCardReader.hpp` - 카드 리더기
- `fakes/FakeBank.hpp` - 은행 시스템
- `fakes/FakeCashBin.hpp` - 현금통


## 새로운 테스트 추가 방법

### 1. 새로운 테스트 케이스 구현

1. `tests/` 디렉토리에 새로운 `xx_tests.cpp` 파일 생성 (ex: `cash_bin_tests.cpp`)
2. 테스트 케이스 작성:
   ```cpp
   TEST(test_cash_withdrawal)
       ...
   END_TEST
   ```

### 2. CMakeLists.txt 업데이트
구현한 테스트 케이스 파일을 `CMakeLists.txt`의 `TEST_FRAMEWORK_SOURCES`에 추가:
```cmake
file(GLOB TEST_FRAMEWORK_SOURCES CONFIGURE_DEPENDS 
    tests/test_framework.cpp
    tests/card_tests.cpp
    tests/banking_tests.cpp


    tests/cash_bin_tests.cpp
)
```

### 3. 테스트 프레임워크 수정
`test_framework.cpp`에 새로운 테스트 함수 추가:
```cpp
extern void test_cash_withdrawal();

void registerAllTests() {
    registerTest("test_card_insert_and_eject", test_card_insert_and_eject);

    ...

    registerTest("test_cash_withdrawal", test_cash_withdrawal);
}
```
