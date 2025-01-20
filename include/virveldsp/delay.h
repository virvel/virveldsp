#pragma once
#include <cstdint>

namespace dsp {
template <int32_t S> class delay_test {
public:
  void write(const float in) {

    m_buffer[m_writePtr] = in;
    m_writePtr = (m_writePtr - 1 + m_maxSize) % m_maxSize;
  }

  float read(const float delay) {
    const auto int_delay = static_cast<int32_t>(delay);
    const float frac = delay - static_cast<float>(int_delay);

    int n = m_writePtr + int_delay + m_maxSize;

    const float y0 = m_buffer[(n - 1) % m_maxSize];
    const float y1 = m_buffer[n % m_maxSize];
    const float y2 = m_buffer[(n + 1) % m_maxSize];
    const float y3 = m_buffer[(n + 2) % m_maxSize];

    const float c0 = y1;
    const float c1 = 0.5f * (y2 - y0);
    const float c3 = 1.5f * (y1 - y2) + 0.5f * (y3 - y0);
    const float c2 = y0 - y1 + c1 - c3;

    return ((c3 * frac + c2) * frac + c1) * frac + c0;
  }

private:
  float m_buffer[S] {};
  const int32_t m_maxSize = S;
  int32_t m_writePtr = 0;
  float m_prev = 0.f;
  float m_mod = 0.f;
};
} /* namespace dsp */
