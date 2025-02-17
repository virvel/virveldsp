#include <iostream>
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <virveldsp/delay.h>
#include <numeric>

TEST(DelayTest, BasicAssertions) {
    std::vector<float> x(100, 0.f);
  std::iota(x.begin(), x.end(), 1.f);
    std::vector<float> y(100, 0.f);

    dsp::delay<10> dd;

    for (size_t i = 0; i < x.size(); i++) {
        dd.write(x[i]);
        y[i] = dd.read(5);
    }

    std::vector<float> expected(100, 0.f);
    std::iota(expected.begin()+4, expected.end(), 1.f);
    EXPECT_THAT(y, testing::Pointwise(testing::FloatNear(1e-5),expected));
}
