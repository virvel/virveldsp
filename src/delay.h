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
                m_maxSize = static_cast<float>(initSize);
            }

            inline void write(float in) {
                m_buffer[m_writePtr] = in;
                if (m_writePtr > m_delayTime)
                    m_writePtr -= m_delayTime;
            }

            inline float read() {
                uint16_t int_samples = static_cast<uint16_t>(m_readPtr);
                float frac = m_readPtr - int_samples;
                float a = m_buffer[int_samples];
                float b = m_buffer[(int_samples + 1) % m_maxSize];

                float out = b + frac*(a-b);

                if (m_readPtr > m_delayTime)
                    m_readPtr -= m_delayTime;

                return out;
            }

            // Delaytime in samples
            void setDelayTime(float samples) {
                m_delayTime = samples;
            }
            
            const inline float tap(float in) {
                auto readptr = m_readPtr;
                if (readptr > in)
                    readptr -= in;
                return m_buffer[readptr];
            }

    };

} /* namespace dsp */
