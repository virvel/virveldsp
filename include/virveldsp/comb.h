#pragma once
#include <cstdint>
namespace dsp {

using UInt = uint16_t;

template <UInt S>
class comb {
public:
  void init(const float ff, const float fb) {
    m_pos = 0;
    m_ff = ff;
    m_fb = fb;
    m_delayTime = static_cast<float>(m_size);
    for (auto &b : m_buf)
      b = 0.f;
  }

  float process(const float in) {
    const float buf_out = m_buf[m_pos % m_size];

    float y = in - m_fb * buf_out;
    m_buf[m_pos] = y;
    const float out = m_ff * y + buf_out;

    m_pos += 1;

    if (m_pos >= m_delayTime)
      m_pos -= m_delayTime;

    return out;
  }
  float tap(uint16_t in) {
    auto pos = m_pos;
    if (pos > in)
      pos -= in;
    return m_buf[pos];
  }

  void setDelayTime(const float delayTime) { m_delayTime = delayTime; }

private:
  uint16_t m_size = S;
  float m_buf[S];
  uint16_t m_pos = 0;
  float m_delayTime = S;
  float m_ff = 0.f;
  float m_fb = 0.f;
};

} /* namespace dsp */
