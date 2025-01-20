add_executable(allpass_test allpass_test.cpp)
target_link_libraries(allpass_test virveldsp GTest::gtest_main GTest::gmock)

gtest_discover_tests(allpass_test)