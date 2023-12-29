#include "daisy.h"

namespace dsp {

    // One pole low pass filter
    class lowpass {
        private:
            uint16_t m_sr;
            float m_fac;
            float m_prev;
            
        public:
            void init(const uint16_t samplerate, const float fac ) {
                m_sr = samplerate;
                m_fac = fac;
            }

            void setFrequency(const float frequency) {
                m_fac = (1.f/m_sr * frequency) / (1.f/m_sr * (frequency + 1));
            }

            void setSmoothingFactor(const float fac) {
                m_fac = fac;
            }
            
            inline float process(const float in) {
                m_prev = m_prev + m_fac * (in - m_prev);
                return m_prev; 
    }

    };

}