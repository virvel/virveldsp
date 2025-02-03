#pragma once
#include <cmath>

namespace dsp {

class lfo {

public:
  static constexpr float twopi = 6.2831853072f;
  void init(const float sampleRate) {
    m_sampleRate = sampleRate;
    m_freq = 1.f;
    reset();
  }

  void reset() {
    m_omega = twopi * (m_freq / m_sampleRate);
    m_e = 2.f * std::sin(m_omega / 2.f);
    m_x = std::cos(m_omega);
    m_y = std::sin(m_omega);
  }

  void setFreq(const float freq) {
    m_freq = freq;
    reset();
  }

  void process() {
    m_x = m_x - m_e * m_y;
    m_y = m_e * m_x + m_y;
  }

  float processCosine() {
    process();
    return m_x;
  }

  float processSine() {
    process();
    return m_y;
  }

private:
  float m_freq = 0.f;
  float m_sampleRate = 0.f;
  float m_omega = 0.f;
  float m_x = 0.f;
  float m_y = 0.f;
  float m_e = 0.f;
};
} // namespace dsp
