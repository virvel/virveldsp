#pragma once
constexpr float twopi = 6.2831853072f;

namespace dsp {

class lfo {

public:
  lfo() = default;

  void init(const float sampleRate) {
    m_sampleRate = sampleRate;
    m_freq = 1.f;
    reset();
  }

  void reset() {
    m_omega = twopi * (m_freq / m_sampleRate);
    m_e = 2.f * sin(m_omega / 2.f);
    m_x = cos(m_omega);
    m_y = sin(m_omega);
  }

  void setFreq(const float freq) {
    m_freq = freq;
    reset();
  }

  inline void process() {
    m_x = m_x - m_e * m_y;
    m_y = m_e * m_x + m_y;
  }

  inline float processCosine() {
    process();
    return m_x;
  }

  inline float processSine() {
    process();
    return m_y;
  }

private:
  float m_freq;
  float m_sampleRate;
  float m_omega;
  float m_x;
  float m_y;
  float m_e;
};
} // namespace dsp
