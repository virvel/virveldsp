#pragma once

#include <cstdint>

namespace dsp {
  using UInt = uint16_t;

  template<UInt S>
  class allpass {
  public:
    allpass() = default;

    void setDelayTime(const float dtime) { m_delayTime = dtime; }

    void init() {
      m_pos = 0;
      m_xprev = 0;
      m_yprev = 0;
      m_delayTime = static_cast<float>(m_size);
    }

    inline float process(const float in) {
      m_pos += 1;

      if (m_pos >= m_delayTime)
        m_pos -= m_delayTime;

      auto pos = static_cast<uint16_t>(m_pos);
      const float frac = m_pos - static_cast<float>(pos);
      const float aa = (1 - frac) / (1 + frac);
      const float xn = m_buf[pos % m_size];
      const float out = aa * (xn - m_yprev) + m_xprev;
      m_xprev = xn;
      m_yprev = out;

      m_buf[pos] = in;

      return out;
    }

  private:
    uint16_t m_size = S;
    float m_buf[S];
    float m_pos = 0.f;
    float m_delayTime = S;
    float m_yprev = 0.f;
    float m_xprev = 0.f;
  };
} // namespace dsp
