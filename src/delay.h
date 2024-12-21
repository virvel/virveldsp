#include <cstdint>

namespace dsp {
    
template <uint16_t S>
class delay {
    private:
        float m_buffer[S];
        const uint32_t m_maxSize = S;
        float m_readPtr = 0.f;
        uint16_t m_writePtr = 0;
        float m_delayTime = S;
        float m_newDelayTime = S;
        float m_prev = 0.f;
        float m_mod = 0.f;

    public: 
        void init(const float initSize) {
            m_delayTime = initSize;
        }

        inline void mod(float mod) {
            m_mod = mod;
        }

        inline void write(const float in) {
            if (m_writePtr >= m_delayTime)
                m_writePtr -= m_delayTime;

            m_buffer[m_writePtr] = in;
            m_writePtr++;
        }

        inline void setDelayTime(const float delayTime) {
            m_newDelayTime = delayTime;
        }

        inline void update() {

            m_delayTime = 0.95 * m_delayTime + 0.05*(m_newDelayTime + m_mod);
        }

        inline float read() {

            m_readPtr += 1;

            update();

            if (m_readPtr >= m_delayTime)
                m_readPtr -= m_delayTime;
            
            const uint32_t int_pos = static_cast<uint32_t>(m_readPtr);
            const float frac = m_readPtr - int_pos;

            if (m_readPtr <= 1)
                return m_buffer[1];
            else if (m_readPtr >= (m_maxSize  - 2))
                return m_buffer[m_maxSize - 3];

            else {
           auto n = int_pos;

           if (n > m_maxSize - 2)
               n = m_maxSize - 3;

            const float y0 = m_buffer[n-1];
            const float y1 = m_buffer[n];
            const float y2 = m_buffer[n+1];
            const float y3 = m_buffer[n+2];

            const float c0 = y1;
            const float c1 = 0.5f * (y2 - y0);
            const float c3 = 1.5f * (y1 - y2) + 0.5f * (y3 - y0);
            const float c2 = y0 - y1 + c1 - c3;

            return ((c3 * frac + c2) * frac + c1) * frac + c0;

            }

        }
        
        inline float tap(const uint16_t in) {
            auto readptr = m_readPtr;
            if (readptr > in)
                readptr -= in;
            return m_buffer[readptr];
        }

};

} /* namespace dsp */
