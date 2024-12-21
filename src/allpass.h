#pragma once
#include <cstdint>

namespace dsp {
using UInt = uint16_t;

template <UInt S> class allpass {
public:
  void init() {
    m_pos = 0;
    m_xprev = 0;
    m_yprev = 0;
    m_delayTime = static_cast<float>(m_size);
  };

  float process(float in) {

    m_pos += 1;

    if (m_pos >= m_delayTime)
      m_pos -= m_delayTime;

    auto pos = m_pos;
    const float frac = m_pos - pos;
    const float aa = (1 - frac) / (1 + frac);
    const float xn = m_buf[pos % m_size];
    const float out = aa * (xn - m_yprev) + m_xprev;
    m_xprev = xn;
    m_yprev = out;

    m_buf[pos] = in;

    return out;
  };
  void setDelayTime(const float dtime) { m_delayTime = dtime; }

private:
  uint16_t m_size = S;
  float m_buf[S];
  uint16_t m_pos = 0;
  float m_delayTime = S;
  float m_yprev = 0.f;
  float m_xprev = 0.f;
};
} // namespace dsp
