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

add_executable(atm tests/test_atm.cpp)
target_link_libraries(atm atm_lib)