#include <cstdint>

namespace dsp {
using UInt = uint16_t;

template <UInt S> class allpass {
public:
  void init();
  inline float process(const float in);
  void setDelayTime(const float dtime) { m_delayTime = dtime;}

private:
  uint16_t m_size = S;
  float m_buf[S];
  uint16_t m_pos;
  float m_delayTime;
  float m_yprev;
  float m_xprev;
};

template <UInt S> void allpass<S>::init() {
  m_pos = 0;
  m_xprev = 0;
  m_yprev = 0;
  m_delayTime = static_cast<float>(m_size);
}

template <UInt S> inline float allpass<S>::process(const float in) {

  m_pos += 1;

  if (m_pos >= m_delayTime)
    m_pos -= m_delayTime;

  uint16_t pos = static_cast<uint16_t>(m_pos);
  float frac = m_pos - pos;
  float aa = (1 - frac) / (1 + frac);
  float xn = m_buf[pos % m_size];
  float out = aa * (xn - m_yprev) + m_xprev;
  m_xprev = xn;
  m_yprev = out;

  m_buf[pos] = in;

  return out;
}

} // namespace dsp
