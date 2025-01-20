#include <iostream>
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <virveldsp/allpass.h>

TEST(AllpassTest, BasicAssertions) {
    std::vector<float> x(30, 0.f);
    x[0] = 1.f;
    std::vector<float> y(30, 0.f);

    dsp::allpass<20> ap;
    ap.setDelayTime(3);

    for (int i = 0; i < x.size(); i++) {
        y[i] = ap.process(x[i]);
    }

    for (int i = 0; i < y.size(); i++) {
      std::cout << y[i] << std::endl;
    }

    std::vector<float> expected(20, 0.f);
    EXPECT_THAT(y, testing::Pointwise(testing::FloatNear(1e-5),expected));
}
