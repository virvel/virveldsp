#include "daisy.h"

namespace dsp {
    
    template <uint16_t S>
    class delay {
        private:
            float m_buffer[S];
            const uint16_t m_maxSize = S;
            uint16_t m_readPtr;
            uint16_t m_writePtr;
            float m_delayTime;

        public: 
            void init(uint16_t initSize) {
                m_delayTime = static_cast<float>(initSize);
            }

            inline void write(float in) {
                if (m_writePtr > m_delayTime)
                    m_writePtr -= m_delayTime;

                m_buffer[m_writePtr] = in;

                m_writePtr++;

            }

            inline float read() {

                uint16_t int_samples = static_cast<uint16_t>(m_readPtr);
                float frac = m_readPtr - int_samples;
                float a = m_buffer[int_samples];
                float b = m_buffer[(int_samples + 1) % m_maxSize];

                float out = b + frac*(a-b);

                m_readPtr = (m_readPtr + 1) % static_cast<uint16_t>(m_delayTime);

                return out;
            }

            // Delaytime in samples
            void setDelayTime(float samples) {
                m_delayTime = samples;
            }
            
            inline float tap(uint16_t in) {
                auto readptr = m_readPtr;
                if (readptr > in)
                    readptr = readptr % in;
                return m_buffer[readptr];
            }

    };

} /* namespace dsp */
