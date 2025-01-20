#include <cstdint>
namespace dsp {

class lowpass {
    private:
        uint16_t m_sr;
        float m_fac;
        float m_newfac;
        float m_prev;

    public:
        void init(const uint16_t samplerate, const float fac ) {
            m_sr = samplerate;
            m_fac = fac;
            m_prev = 0.f;
        }

        void setFrequency(const float frequency) {
            m_fac = (1.f/m_sr * frequency) / (1.f/m_sr * (frequency + 1));
        }

        void setSmoothingFactor(const float fac) {
            m_newfac = fac;
        }

        void update() {

            m_fac = 0.5*m_fac + 0.5*m_newfac;
        }

        inline float process(float in) {
            update();
            m_prev = in - m_fac * m_prev;
            return m_prev;
}

};

}
