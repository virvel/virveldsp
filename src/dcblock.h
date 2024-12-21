#pragma once

namespace dsp {

struct dcblock {
  float process(const float in) {
    float y = in - xn1 + 0.997f * yn1;

    yn1 = y;
    xn1 = in;
    return y;
  }

  float xn1 = 0.f;
  float yn1 = 0.f;
};

}