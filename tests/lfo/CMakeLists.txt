add_executable(lfo_test lfo_test.cpp)
target_link_libraries(lfo_test virveldsp GTest::gtest_main GTest::gmock)

gtest_discover_tests(lfo_test)