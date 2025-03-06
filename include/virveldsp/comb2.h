#pragma once
namespace dsp {

    using UInt = unsigned;

    template <UInt S>
    class comb2 {
    public:
        void init(const float ff, const float fb) {
            ptr = 0;
            m_ff = ff;
            m_fb = fb;
            m_delayTime = static_cast<float>(maxSize);
            for (auto &b : buffer)
                b = 0.f;
        }

        void write(const float in) {
          buffer[ptr] = in;
          ptr = (ptr - 1 % maxSize) % maxSize;
          }

        float process(const float in) {
            const auto int_delay = static_cast<unsigned>(delay);
            const float frac = delay - static_cast<float>(int_delay);

            const int n = ptr + int_delay + maxSize;

            const float y0 = buffer[(n - 1) % maxSize];
            const float y1 = buffer[n % maxSize];
            const float y2 = buffer[(n + 1) % maxSize];
            const float y3 = buffer[(n + 2) % maxSize];

            const float c0 = y1;
            const float c1 = 0.5f * (y2 - y0);
            const float c3 = 1.5f * (y1 - y2) + 0.5f * (y3 - y0);
            const float c2 = y0 - y1 + c1 - c3;

            const float buf_out = ((c3 * frac + c2) * frac + c1) * frac + c0;

            float y = in - m_fb * buf_out;
            buffer[ptr] = y;
            const float out = m_ff * y + buf_out;

            return out;
        }

        void setDelayTime(const float t) {delay = t;}

    private:
        unsigned maxSize = S;
        float buffer[S];
        float delay = static_cast<float>(S);
        unsigned ptr = 0;
        float m_delayTime = S;
        float m_ff = 0.f;
        float m_fb = 0.f;
    };

} /* namespace dsp */
