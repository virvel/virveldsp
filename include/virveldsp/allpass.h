#pragma once
#include <cstdint>

namespace dsp {
using UInt = uint16_t;

template <UInt S> class allpass {
public:
  void init() { m_delayTime = static_cast<float>(m_size); }

  void setDelayTime(const float dtime) { m_delayTime = dtime; }

  float process(const float in) {
    const auto int_del = static_cast<uint16_t>(m_delayTime);
    const float frac = m_delayTime - static_cast<float>(int_del);
    int t = m_pos + static_cast<uint16_t>(int_del) + m_size;

    const float aa = (1 - frac) / (1 + frac + 0.001);
    const float ynl = m_buf[t % m_size];
    const float vm = in - ynl * aa;
    const float out = aa * vm + ynl;

    m_buf[m_pos] = vm;

    m_pos = (m_pos - 1 + m_size) % m_size;

    return out;
  }

private:
  uint16_t m_size = S;
  float m_buf[S]{};
  uint16_t m_pos = 0;
  float m_delayTime = S;
  float m_yprev = 0.f;
  float m_xprev = 0.f;
};

} // namespace dsp
