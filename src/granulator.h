#pragma once
#include "daisy.h"
#include "daisysp.h"

namespace dsp {

    class Grain
    {

    public:
        void init(float * const buffer, uint32_t size);
        float play();
        void setActive(const bool active) { m_active = active; }
        void setRate(const float rate) { m_rate = rate; }
        void setOffset(const float pos) { m_next_offset = static_cast<uint32_t>(daisysp::fmin(pos * float((m_size - m_duration)), float(m_size - m_duration))); }
        void setJitter(const float jitter) { m_jitter = static_cast<uint32_t>(jitter * static_cast<float>(m_size)); }
        void setDelay(const uint32_t delayInSamples) {m_delay = delayInSamples;}
        void setDuration(const float s);
        void setSize(const uint32_t size) { m_size = size;}
        uint32_t getDuration() { return m_duration;}

    private:
        bool m_active;
        float *m_buf;
        float m_position;
        float m_rate;
        uint32_t m_offset;
        uint32_t m_next_offset;
        uint32_t m_duration;
        uint32_t m_size;
        uint32_t m_jitter;
        uint32_t m_delay;
    };

    class Granulator
    {

    public:
        void init(float * const buffer, const uint32_t size);
        void setRate(const float speed);
        void setDuration(const float s);
        void setOffset(const float o);
        void setJitter(const float s);
        float play();
        void play(float * outL, float * outR);
        void setNumSamples(const uint32_t s);
        uint32_t getNumSamples(void) { return m_numSamples; }

    private:
        float *m_buffer;
        float m_spray;
        uint32_t m_numSamples;
        const uint8_t m_num_grains = 12;
        Grain m_grainsLeft[12];
        Grain m_grainsRight[12];
    };

}; /* namespace dsp */