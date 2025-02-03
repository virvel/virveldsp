#include <iostream>
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <virveldsp/lfo.h>

TEST(LFOTest, BasicAssertions) {
    std::vector<float> x(4800, 0.f);
    std::vector<float> y(4800, 0.f);

    dsp::lfo lfo;
    lfo.init(48000.f);
    lfo.setFreq(10.f);

    for (size_t i = 0; i < x.size(); i++) {
        x[i] = lfo.processCosine();
    }

    for (size_t i = 0; i < 10000*x.size(); i++) {
      auto _ = lfo.processCosine();
    }

    for (size_t i = 0; i < y.size(); i++) {
      y[i] = lfo.processCosine();
    }

    EXPECT_THAT(x, testing::Pointwise(testing::FloatNear(1e-5),y));
}
