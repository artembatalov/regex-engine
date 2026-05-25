cmake_minimum_required(VERSION 3.16)
project(regex-engine VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        v1.14.0
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_library(regex_engine_lib
    src/parser.cpp
    src/nfa.cpp
    src/dfa.cpp
    src/regex_engine.cpp
)
target_include_directories(regex_engine_lib PUBLIC include)

enable_testing()

add_executable(regex_tests
    tests/test_parser.cpp
    tests/test_nfa.cpp
    tests/test_dfa.cpp
    tests/test_matching.cpp
)
target_link_libraries(regex_tests PRIVATE regex_engine_lib GTest::gtest_main)
include(GoogleTest)
gtest_discover_tests(regex_tests)

add_executable(example_matching examples/example_matching.cpp)
target_link_libraries(example_matching PRIVATE regex_engine_lib)

add_executable(example_counting examples/example_counting.cpp)
target_link_libraries(example_counting PRIVATE regex_engine_lib)
