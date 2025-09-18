cmake_minimum_required(VERSION 3.12)
project(atm-controller LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/*.cpp)
add_library(atm_lib ${SOURCES})
target_include_directories(atm_lib PUBLIC ${CMAKE_SOURCE_DIR}/include)

if (MSVC)
    target_compile_options(atm_lib PRIVATE /W4)
else()
    target_compile_options(atm_lib PRIVATE -Wall -Wextra -O2)
endif()

file(GLOB TEST_FRAMEWORK_SOURCES CONFIGURE_DEPENDS 
    tests/test_framework.cpp
    tests/card_tests.cpp
    tests/banking_tests.cpp
    tests/exception_tests.cpp
    tests/transaction_tests.cpp
)

add_executable(atm tests/test_runner.cpp ${TEST_FRAMEWORK_SOURCES})
target_link_libraries(atm atm_lib)
target_include_directories(atm PRIVATE
    ${CMAKE_SOURCE_DIR}/tests
    ${CMAKE_SOURCE_DIR}/tests/fakes
)