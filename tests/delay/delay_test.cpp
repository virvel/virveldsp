#include <iostream>
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <virveldsp/delay.h>

TEST(DelayTest, BasicAssertions) {
    std::vector<float> x(20, 0.f);
    x[0] = 1.f;
    x[1] = -0.5f;
    std::vector<float> y(20, 0.f);

    dsp::delay<10> dd;

    for (size_t i = 0; i < x.size(); i++) {
        dd.write(x[i]);
        y[i] = dd.read(5);
    }

    std::vector<float> expected(20, 0.f);
    expected[4] = 1.f;
    expected[5] = -0.5f;
    EXPECT_THAT(y, testing::Pointwise(testing::FloatNear(1e-5),expected));
}
