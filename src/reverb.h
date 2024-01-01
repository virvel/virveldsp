#include "daisy.h"
#include "comb.h"
#include "delay.h"
#include "lowpass.h"

namespace dsp {

class reverb {

    private:
        float m_decay;
        float m_bandwidth;
        uint16_t m_predelay;
        float m_prevLeft;
        float m_prevRight;

        dsp::comb<142> input_diffusion11;
        dsp::comb<107> input_diffusion12;
        dsp::comb<379> input_diffusion21;
        dsp::comb<277> input_diffusion22;
        dsp::comb<672> decay_diffusion11;
        dsp::comb<908> decay_diffusion12;
        dsp::comb<1800> decay_diffusion21;
        dsp::comb<2656> decay_diffusion22;
        dsp::delay<4453> delay1;
        dsp::delay<4217> delay2;
        dsp::delay<3720> delay3;
        dsp::delay<3163> delay4;
        dsp::delay<24000> predelay;
        dsp::lowpass bandwidth;
        dsp::lowpass damping1;
        dsp::lowpass damping2;


    public:

        void init() {
            input_diffusion11.init(0.7f, 0.7f);
            input_diffusion12.init(0.7f, 0.7f);
            input_diffusion21.init(0.5f, 0.5f);
            input_diffusion22.init(0.5f, 0.5f);
            decay_diffusion11.init(0.75f, 0.75f);
            decay_diffusion12.init(0.75f, 0.75f);
            decay_diffusion21.init(0.625f, 0.625f);
            decay_diffusion22.init(0.625f, 0.625f);

            delay1.init(4453);
            delay2.init(4217);
            delay3.init(3720);
            delay4.init(3163);
            predelay.init(1000);

            bandwidth.init(48000, 0.5);
            damping1.init(48000, 0.5);
            damping2.init(48000, 0.5);

            m_prevLeft = 0.f;
            m_prevRight = 0.f;
            m_decay = 0.95;


        }
        

        void process(float inLeft, float inRight, float * outLeft, float * outRight) {
            float in = 0.5 * (inLeft + inRight);
            float yl = 0.f;
            float yr = 0.f;

            predelay.write(in);
            in = predelay.read();
            in = bandwidth.process(in);
            in = input_diffusion11.process(in);
            in = input_diffusion12.process(in);
            in = input_diffusion21.process(in);
            in = input_diffusion22.process(in);

            float left = decay_diffusion11.process(in + m_decay*m_prevRight);
            delay1.write(left);
            left = delay1.read();
            yr += 0.6 * delay1.tap(353);
            yr -= 0.6 * delay1.tap(3627);
            yl -= 0.6 * delay1.tap(1990);
            left = m_decay*damping1.process(left);
            left = decay_diffusion21.process(left);
            yr += decay_diffusion21.tap(1228);
            yl -= decay_diffusion21.tap(187);
            delay3.write(left);
            left = delay3.read();
            yr -= delay3.tap(2673);
            yl -= delay3.tap(1066);

            float right = decay_diffusion12.process(in + m_decay*m_prevLeft);
            delay2.write(right);
            right = delay2.read();
            yl += 0.6*delay2.tap(266);
            yl -= 0.6 * delay2.tap(2974);
            yr -= 0.6 * delay2.tap(2111);
            right = m_decay*damping2.process(right);
            right = decay_diffusion22.process(right);
            yl += 0.6 * decay_diffusion22.tap(1913);
            yr -= 0.6 * decay_diffusion22.tap(335);
            delay4.write(right);
            right = delay4.read();
            yl -= 0.6 * delay4.tap(1996);
            yr -= 0.6 * delay4.tap(121);


            m_prevLeft = left;
            m_prevRight = right;

            *outLeft = yl;
            *outRight = yr;

        }

};
    
} /* namespace dsp */