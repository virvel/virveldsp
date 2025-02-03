#pragma once

#include <array>
#include <algorithm>
#include <cmath>

namespace dsp {
            
template <int numHarmonics>
class Chebyshev {

public:
    void init() {
        std::fill(amplitudes.begin(), amplitudes.end(), 1.f);
    }

    float process(float in) {
        in = std::fmin(std::fmax(-1.f, in), 1.f);

        float prev = in;
        float curr = 2.f * in * in - 1.f;

        float tmp = 0.f;
        float sum = amplitudes[0]*prev + amplitudes[1]*curr;

        for (int i = 2; i < numHarmonics; ++i) {
            tmp = curr;
            curr = 2.f*in*curr - prev;
            prev = tmp;

            sum += amplitudes[i] * curr;
         }

        return sum/numHarmonics;
    }

    void setAmplitude(const int n, const float w) {
        amplitudes[n] = w;
    }

    const auto& getAmplitudes() const {
            return &amplitudes;
    }

    auto& getAmplitudes() {
            return &amplitudes;
    }
private:
    std::array<float, numHarmonics> amplitudes;
};
} /* namespace daisysp */
