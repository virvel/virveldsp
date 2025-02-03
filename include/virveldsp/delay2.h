#pragma once

#include <cstdint>
#include <cassert>
#include <cmath>
namespace dsp {
    
template <uint16_t S>
class delay2 {
    private:
        float m_buffer[S];
        const uint32_t m_maxSize = S;
        float m_readPtr;
        uint16_t m_writePtr;
        float m_delayTime;
        float m_newDelayTime;
        float m_prev;
        float m_mod;

    public: 
        void init(const float initSize) {
            assert(initSize < S);
            for (uint32_t i = 0; i < m_maxSize; ++i)
                m_buffer[i] = 0;
            m_writePtr = 0;
            m_readPtr = static_cast<float>(m_maxSize -1);
        }

        inline void mod(float mod) {
            m_mod = mod;
        }

        inline void setDelayTime(const float delayTime) {
            m_readPtr = static_cast<float>(m_writePtr) - delayTime;
            while (m_readPtr <= 0) m_readPtr += m_maxSize;
        }

        inline float read(float in) {
            
            m_buffer[m_writePtr++] = in;

            const uint32_t int_pos = static_cast<uint32_t>(std::floor(m_readPtr));
            const float frac = m_readPtr - static_cast<float>(int_pos);

            auto n = int_pos;

            m_readPtr += 1.f;

            if (m_readPtr >= m_maxSize) m_readPtr -= m_maxSize;
            if (m_writePtr >= m_maxSize) m_writePtr -= m_maxSize;

           return frac*m_buffer[n] + (1-frac)*m_buffer[(n+1)%m_maxSize];

        }

};

} /* namespace dsp */
