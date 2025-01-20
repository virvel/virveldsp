#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <iostream>
#include <virveldsp/comb.h>

TEST(CombTest, BasicAssertions) {
  std::vector<float> x(30, 0.f);
  x[0] = 1.f;
  std::vector<float> y(30, 0.f);

  dsp::comb<20> comb;
  comb.init(0.2, 0.2);
  comb.setDelayTime(4.f);

  for (int i = 0; i < x.size(); i++) {
    y[i] = comb.process(x[i]);
  }

  for (int i = 0; i < y.size(); i++) {
    std::cout << y[i] << std::endl;
  }

  std::vector<float> expected(20, 0.f);
  EXPECT_THAT(y, testing::Pointwise(testing::FloatNear(1e-5), expected));
}
