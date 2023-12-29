#include "daisy.h"

namespace dsp {

using UInt = uint16_t;

template <UInt S>
class comb {
    public:
        void init(const float ff = 0.f, const float fb = 0.f);
        const inline float process(const float in);
        const inline float tap(float in);
        inline void mod(float modulator);
    private:
        uint16_t m_size = S;
        float m_buf[S];
        float m_prevy;
        uint16_t m_pos;
    protected:
        float m_ff;
        float m_fb;
};


template <UInt S>
void comb<S>::init(const float ff, const float fb) { 
    m_ff = ff;
    m_fb = fb;
    m_prevy = 0.f;
}

template <UInt S>
inline void comb<S>::mod(const float modulator) {
    m_size += static_cast<uint16_t>(modulator);
}

template <UInt S>
const inline float comb<S>::process(const float in) {

    const float buf_out = m_buf[m_pos];
    m_prevy = buf_out - in * m_fb;
    m_buf[m_pos] = m_prevy;
    const float out = m_ff * m_prevy + buf_out;

    m_pos++;
    if (m_pos >= m_size)
        m_pos -= m_size;
    
    return out;
}

template <UInt S>
const inline float comb<S>::tap(float in) {
    auto pos = m_pos;
    if (pos > in)
        pos -= in;
    return m_buf[pos];
}

} /* namespace dsp */