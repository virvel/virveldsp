#include "granulator.h"
#include <cmath>

void dsp::Grain::init(float* const buffer, const uint32_t size) {
  m_buf = buffer;
  m_active = false;
  m_position = 0;
  m_size = size;
  m_rate = 1.f;
  m_duration = m_size;
  m_offset = 0;
  m_next_offset = 0;
}

float dsp::Grain::play() {

  if (m_position >= m_duration) {
    m_active = false;
    m_delay = static_cast<uint32_t>(
        static_cast<float>(m_duration / 2) *
        static_cast<float>(rand() / static_cast<float>(RAND_MAX)));
  }

  if (m_active) {

    m_position = m_position + m_rate;

    const uint32_t int_pos = static_cast<uint32_t>(m_position);
    const float frac = m_position - int_pos;

    auto c = float(int_pos % m_duration) / float(m_duration);
    c = std::sin(M_PI * c);

    if (m_position <= 1)
      return c * m_buf[1];
    else if (m_position >= (m_size - 2))
      return c * m_buf[m_size - 2];

    else {
      auto n = int_pos + m_offset;

      if (n > m_size - 2)
        n = m_size - 3;

      const float y0 = m_buf[n - 1];
      const float y1 = m_buf[n];
      const float y2 = m_buf[n + 1];
      const float y3 = m_buf[n + 2];

      const float c0 = y1;
      const float c1 = 0.5f * (y2 - y0);
      const float c3 = 1.5f * (y1 - y2) + 0.5f * (y3 - y0);
      const float c2 = y0 - y1 + c1 - c3;

      return c * (((c3 * frac + c2) * frac + c1) * frac + c0);
    }

  } else {
    if (m_delay > 0) {
      m_delay--;
      return 0.f;
    }
    m_position = 0.f;
    m_offset = m_next_offset +
               static_cast<uint32_t>(
                   static_cast<float>(m_jitter) *
                   static_cast<float>(rand() / static_cast<float>(RAND_MAX)));
    m_active = true;
    return 0.f;
  }
}

void dsp::Grain::setDuration(const float s) {
  {
    m_duration =
        std::max(static_cast<uint32_t>(10),
                 static_cast<uint32_t>(s * static_cast<float>(m_size)));
  }
}

void dsp::Granulator::init(float* const buffer, const uint32_t numSamples) {
  m_numSamples = numSamples / 2;
  m_buffer = buffer;

  for (auto& g : m_grainsLeft)
    g.init(buffer, numSamples / 2);
  for (auto& g : m_grainsRight)
    g.init(&buffer[numSamples / 2], numSamples / 2);
}

void dsp::Granulator::setOffset(const float offset) {
  for (auto& g : m_grainsLeft)
    g.setOffset(offset);
  for (auto& g : m_grainsRight)
    g.setOffset(offset);
}

void dsp::Granulator::setNumSamples(const uint32_t size) {
  m_numSamples = size;
  for (auto& g : m_grainsLeft)
    g.setSize(size);
  for (auto& g : m_grainsRight)
    g.setSize(size);
}

void dsp::Granulator::setDuration(const float s) {
  for (auto& g : m_grainsLeft)
    g.setDuration(s);
  for (auto& g : m_grainsRight)
    g.setDuration(s);
}

void dsp::Granulator::setRate(const float speed) {
  for (auto& g : m_grainsLeft)
    g.setRate(speed);
  for (auto& g : m_grainsRight)
    g.setRate(speed);
}

void dsp::Granulator::setJitter(const float jitter) {
  for (auto& g : m_grainsLeft)
    g.setJitter(jitter);
  for (auto& g : m_grainsRight)
    g.setJitter(jitter);
}

void dsp::Granulator::play(float* outL, float* outR) {

  for (auto& g : m_grainsLeft) {
    *outL += g.play();
  }
  for (auto& g : m_grainsRight) {
    *outR += g.play();
  }
}
