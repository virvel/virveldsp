namespace dsp {

template <uint16_t S>
class allpass {
    public:
        void init() { 
            m_pos = 0;
            m_xprev = 0.f;
            m_yprev = 0.f;
        }

        inline void write(const float in) {
            m_buf[m_pos] = in;
            m_pos  = (m_pos + 1) % m_size;
        }

        inline float process(const float delayTime) {

            float floatpos = m_pos + delayTime;

            uint16_t pos = static_cast<uint16_t>(floatpos);
            float frac = floatpos-pos;
            float aa = (1-frac)/(1+frac);
            float xn = m_buf[pos%m_size];
            float out = aa * (xn - m_yprev) + m_xprev;
            m_xprev = xn;
            m_yprev = out;

            return out;
        }
        
    private:
        uint16_t m_size = S;
        float m_buf[S];
        uint16_t m_pos;
        float m_yprev;
        float m_xprev;
};
}
