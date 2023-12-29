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

            bandwidth.init(48000, 0.5);
            damping1.init(48000, 0.5);
            damping2.init(48000, 0.5);

            m_prevLeft = 0.f;
            m_prevRight = 0.f;


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

            float left = decay_diffusion11.process(in + m_prevRight);
            yr = 0.6 * decay_diffusion11.tap(353);
            delay1.write(left);
            yr -= 0.6 * delay1.tap(3627);
            left = delay1.read();
            left = damping1.process(left);
            left = decay_diffusion21.process(left);
            delay3.write(left);
            left = delay3.read();

            float right = decay_diffusion12.process(in + m_prevLeft);
            yl = 0.6*decay_diffusion12.tap(266);
            delay2.write(right);
            yl -= 0.6 * delay2.tap(2974);
            right = delay2.read();
            right = damping2.process(right);
            right = decay_diffusion22.process(right);
            delay4.write(right);
            right = delay4.read();


            m_prevLeft = left;
            m_prevRight = right;

            *outLeft = left;
            *outRight = right;
        }

};
    
} /* namespace dsp */