#include "tusenskona/src/tusenskona.h"
#include "daisysp.h"
#include "dev/mpr121.h"
#include "chebyshev.h"
#include "comb.h"
#include "delay.h"
#include "lowpass.h"
//#include "dcblock.h"

using namespace daisy;
using namespace daisysp;

Mpr121I2CTransport i2c;
Mpr121I2C touch;
Tusenskona hw;

uint16_t touched;

dsp::comb<300> aps[4];
dsp::delay<3800> ds[4];
dsp::lowpass lps[4];

Oscillator sine;
Oscillator lfo;

DcBlock dcb;

float decay = 0.5;

float a=0,b=0,c=0,d=0;

float ctrls[4] = {0,0,0,0};

void AudioCallback(AudioHandle::InputBuffer in, AudioHandle::OutputBuffer out, size_t size)
{
	for (size_t i = 0; i < size; i++)
	{
		out[0][i] = 0;
		out[1][i] = 0; 
        
        float s = sine.Process();
        
        a = lps[0].process(s + a);
        b = lps[1].process(s + c);
        c = lps[2].process(b);
        d = lps[3].process(d);

        a = aps[0].process(a);
        b = aps[1].process(b);
        c = aps[2].process(c);
        d = aps[3].process(d);

        auto mod = (1.f + lfo.Process())/2;
        for (int j = 0; j < 4;++j)
        {
            ds[j].mod(mod*16);
        }

        ds[0].write(a);
        a = ds[0].read();
        ds[1].write(b);
        b = ds[1].read();
        ds[2].write(c);
        c = ds[2].read();
        ds[3].write(d);
        d = ds[3].read();

        a = tanhf(a);
        b = tanhf(b);
        c = tanhf(c);
        d = tanhf(d);

        float aa = (a-b);
        float bb = (a+b);
        float cc = (c-d);
        float dd = (c+d);
        a =decay*(aa-cc);
        b =decay*(aa+cc);
        c =decay*(bb-dd);
        d =decay*(bb+dd);

        a = dcb.Process(a);
        b = dcb.Process(b);
        c = dcb.Process(c);
        d = dcb.Process(d);

        out[0][i] = a;
        out[1][i] = b; 
    }
}

void UpdateControls() {
       hw.ProcessAllControls();
       float ctrl0 = hw.GetKnobValue((daisy::Tusenskona::Knob)0);
       float ctrl1 = hw.GetKnobValue((daisy::Tusenskona::Knob)1);
       float ctrl2 = hw.GetKnobValue((daisy::Tusenskona::Knob)2);
       float ctrl3 = hw.GetKnobValue((daisy::Tusenskona::Knob)3);

       
       if (abs(ctrls[0] - ctrl0) > 0.001) {
           if (ctrl0 < 0.0001)
                           ctrl0 = 0.f;
                   ctrls[0] = ctrl0 + 0.1;
       }
       if (abs(ctrls[1] - ctrl1) > 0.001) {
               if (ctrl1 < 0.0001)
                               ctrl1 = 0.f;
                       ctrls[1] = ctrl1;
       }
           
       if (abs(ctrls[2] - ctrl2) > 0.001) {
                   if (ctrl2 < 0.0001)
                                   ctrl2 = 0.f;
                           ctrls[2] = ctrl2;
       }

       if (abs(ctrls[3] - ctrl3) > 0.001) {
           if (ctrl3 < 0.0001)
                ctrl3 = 0.f;
            ctrls[3] = ctrl3;
       }
        /*
       aps[0].setDelayTime(19 * cc);
       aps[1].setDelayTime(11 * cc);
       aps[2].setDelayTime(33 * cc);
       aps[3].setDelayTime(41 * cc);
       */
       
       ds[0].setDelayTime(10 + 1323 * ctrls[0]);
       ds[1].setDelayTime(10 + 1779 * ctrls[0]);
       ds[2].setDelayTime(10 + 3397 * ctrls[0]);
       ds[3].setDelayTime(10 + 2391 * ctrls[0]);

       decay = ctrls[1];

       lps[0].setSmoothingFactor(ctrls[2]);
       lps[1].setSmoothingFactor(ctrls[2]);
       lps[2].setSmoothingFactor(ctrls[2]);
       lps[3].setSmoothingFactor(ctrls[2]);


       sine.SetFreq(ctrls[3] * 1000);

}

int main(void)
{

    auto fpscr = __get_FPSCR();
        fpscr |= 0x0100000;
            __set_FPSCR(fpscr);

	hw.Init();


    dcb.Init(48000);

    auto i2cconfig = Mpr121I2CTransport::Config();
    i2c.Init(i2cconfig);

    auto mprconfig = Mpr121I2C::Config();
    touch.Init(mprconfig);


    for (int i = 0;i<4;++i) {
        aps[i].init(0.5,0.5);
        ds[i].init(300*(i+1));
        lps[i].init(48000, 0.5);
    }

    aps[0].setDelayTime(19);
    aps[1].setDelayTime(11);
    aps[2].setDelayTime(33);
    aps[3].setDelayTime(41);

    ds[0].setDelayTime(1323);
    ds[1].setDelayTime(1779);
    ds[2].setDelayTime(3397);
    ds[3].setDelayTime(2391);

    sine.Init(48000);
    sine.SetWaveform(5);
    sine.SetFreq(110);

    lfo.Init(48000);
    lfo.SetFreq(0.0005);


	hw.SetAudioBlockSize(16); 
	hw.SetAudioSampleRate(SaiHandle::Config::SampleRate::SAI_48KHZ);
    hw.StartAdc();
	hw.StartAudio(AudioCallback);

	while(1) {
        touched = touch.Touched();
        UpdateControls();
    }
}
