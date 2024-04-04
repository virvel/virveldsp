#include "daisy.h"

namespace dsp {

using UInt = uint16_t;

template <UInt S>
class allpass {
    public:
        void init();
        inline float process(const float in);
        inline float tap(uint16_t in);
        inline void mod(float mod);
        inline void setDelayTime(const float delayTime);
        inline void update();
    private:
        uint16_t m_size = S;
        float m_buf[S];
        uint16_t m_pos;
        float m_delayTime;
        float m_yprev;
        float m_xprev;
        float m_mod;
        float m_newDelayTime;
};


template <UInt S>
void allpass<S>::init() { 
    m_pos = 0;
    m_xprev = 0;
    m_yprev = 0;
    m_delayTime = static_cast<float>(m_size);
}


template <UInt S>
inline void allpass<S>::mod(float mod) {
    m_mod = mod;
}


template <UInt S>
inline void allpass<S>::setDelayTime(const float delayTime) {
    m_newDelayTime = delayTime;
}


template <UInt S>
inline void allpass<S>::update() {
    m_delayTime = 0.995*m_delayTime + 0.005*(m_newDelayTime+m_mod); 
}


template <UInt S>
inline float allpass<S>::process(const float in) {

    m_pos += 1;
     
    if (m_pos >= m_delayTime)
        m_pos -= m_delayTime;

    update();

    uint16_t pos = static_cast<uint16_t>(m_pos);
    float frac = m_pos-pos;
    float aa = (1-frac)/(1+frac);
    float xn = m_buf[pos%m_size];
    float out = aa * (xn - m_yprev) + m_xprev;

    m_xprev = xn;
    m_yprev = out;

    m_buf[pos] = in;

    return out;
}


template <UInt S>
inline float allpass<S>::tap(uint16_t in) {
    auto pos = m_pos;
    if (pos > in)
        pos -= in;
    return m_buf[pos];
}

}
