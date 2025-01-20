add_executable(comb_test comb_test.cpp)
target_link_libraries(comb_test virveldsp GTest::gtest_main GTest::gmock)

gtest_discover_tests(comb_test)