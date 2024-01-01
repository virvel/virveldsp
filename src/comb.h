#include "daisy.h"

namespace dsp {

using UInt = uint16_t;

template <UInt S>
class comb {
    public:
        void init(const float ff = 0.f, const float fb = 0.f);
        inline float process(const float in);
        inline float tap(uint16_t in);
        inline void mod(float modulator);
    private:
        uint16_t m_size = S;
        float m_buf[S];
        uint16_t m_pos;
    protected:
        float m_ff;
        float m_fb;
};


template <UInt S>
void comb<S>::init(const float ff, const float fb) { 
    m_pos = 0;
    m_ff = ff;
    m_fb = fb;
}

template <UInt S>
inline void comb<S>::mod(const float modulator) {
    m_size += static_cast<uint16_t>(modulator);
}

template <UInt S>
inline float comb<S>::process(const float in) {

    float buf_out = m_buf[m_pos];
    float y = in - buf_out* m_fb;
    m_buf[m_pos] = y;
    const float out = m_ff * y + buf_out;

    m_pos = (m_pos + 1) % m_size;
    
    return out;
}

template <UInt S>
inline float comb<S>::tap(uint16_t in) {
    auto pos = m_pos;
    if (pos > in)
        pos = pos % in;
    return m_buf[pos];
}

} /* namespace dsp */