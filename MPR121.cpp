#include "daisy_seed.h"
#include "daisysp.h"
#include "dev/mpr121.h"
#include "chebyshev.h"
#include "comb.h"
#include "delay.h"
#include "lowpass.h"

using namespace daisy;
using namespace daisysp;

Mpr121I2CTransport i2c;
Mpr121I2C touch;
DaisySeed hw;

uint16_t touched;

dsp::comb<300> aps[4];
dsp::delay<3000> ds[4];
dsp::lowpass lps[4];


float decay =0.5;

float a,b,c,d;

void AudioCallback(AudioHandle::InputBuffer in, AudioHandle::OutputBuffer out, size_t size)
{
	for (size_t i = 0; i < size; i++)
	{
		out[0][i] = 0.f;
		out[1][i] = 0.f;

        float s = (in[0][i]+in[1][i])*0.5;

        a = lps[0].process(s + a);
        b = lps[1].process(s + c);
        c = lps[2].process(b);
        d = lps[3].process(d);

        a = aps[0].process(a);
        b = aps[1].process(b);
        c = aps[2].process(c);
        d = aps[3].process(d);

        ds[0].write(a);
        a = ds[0].read();
        ds[1].write(b);
        b = ds[1].read();
        ds[2].write(c);
        c = ds[2].read();
        ds[3].write(d);
        d = ds[3].read();

        float aa = (a-b);
        float bb = (a+b);
        float cc = (c-d);
        float dd = (c+d);
        a =tanhf(decay*(aa-cc));
        b =tanhf(decay*(aa+cc));
        c =tanhf(decay*(bb-dd));
        d =tanhf(decay*(bb+dd));

        out[0][i] = a;
        out[1][i] = b;
        
        /*
        if (touched > 0 ) {
            for (int j = 0; j < 12;j++) {
                out[0][i] += ((touched >> j) & 1) *  sines[j].Process();
                out[1][i] = out[0][i];
            }
        }*/
	}
}

int main(void)
{

	hw.Init();
	hw.SetAudioBlockSize(16); 
    auto i2cconfig = Mpr121I2CTransport::Config();
    i2c.Init(i2cconfig);

    auto mprconfig = Mpr121I2C::Config();
    touch.Init(mprconfig);

    for (int i = 0;i<4;++i) {
        aps[i].init(0.5,0.5);
        ds[i].init(300*(i+1));
        lps[i].init(48000, 0.5);

    }

    /*
    for (int i = 0; i < 12; i++)
    {
        sines[i].Init(48000);
        sines[i].SetFreq(110*(i+1));
        sines[i].SetAmp(0.085);
    }
    */

	hw.SetAudioSampleRate(SaiHandle::Config::SampleRate::SAI_48KHZ);
	hw.StartAudio(AudioCallback);

	while(1) {
        touched = touch.Touched();
    }
}
