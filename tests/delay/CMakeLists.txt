add_executable(delay_test delay_test.cpp)
target_link_libraries(delay_test virveldsp GTest::gtest GTest::gtest_main GTest::gmock)

gtest_discover_tests(delay_test)