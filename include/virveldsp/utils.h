#pragma once
#include <cmath>
#include <algorithm>

namespace dsp {

inline float clamp(float in, float min, float max) {
  float r;
#ifdef __arm__
  asm("vmaxnm.f32 %[d], %[n], %[m]"
      : [d] "=t"(r)
      : [n] "t"(in), [m] "t"(min)
      :);
  asm("vminnm.f32 %[d], %[n], %[m]" : [d] "=t"(r) : [n] "t"(r), [m] "t"(max) :);
#else
  r = std::min(std::max(in, min), max);
#endif
  return r;
}

inline float tanh_(float x) {
  x = clamp(x, -1.f, 1.f);
  const float twox = x * x;
  x = x + 0.16489087f * twox * x + 0.00985468f * twox * twox * x;
  return x / std::sqrt(1.f + x * x);
}

inline float tanhy_(float x) {
  x = clamp(x, -1.f, 1.f);
  const float twox = x*x;
  const float twoxx = twox*x;
  const float fivex = twoxx*twox;
  x += 0.16489087f * twoxx;
  x += 0.00985468f * fivex;
  float sqt = 1.f + x * x;
#ifdef __arm__
  asm("vsqrt.f32 %[d], %[n]" : [d] "=t"(sqt) : [n] "t"(sqt):);
#else
  sqr = std::sqrt(sqt);
#endif
  return x / sqt;
}

// log : [0,1] -> [0,1]
inline float plog(const float in) {
  constexpr float recip = 1 / 3.f;
  return recip * std::log10(in) + 1.f;
}

} // namespace dsp
