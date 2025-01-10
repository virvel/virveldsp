#pragma once
#include <cmath>

namespace dsp {

struct onepole {
  float process(const float in) {
    float y = b * in - a * yn1;
    yn1 = y;
    return y;
  }

  void SetFrequency(float f) {
    //    f = f < 0.49f ? f : 0.49f;
    a = -1.f + f;
    b = 1.f - std::abs(a);
  }

  float yn1 = 0.f;
  float a = -0.5f;
  float b = 1.f - std::abs(a);
};

} // namespace dsp