/*
 * Frequency shifter using FFT based Hilbert transform
 */
#pragma once

#include "arm_math.h"
#include <cstdint>

namespace daisysp {

class FrequencyShifter {
public:
  FrequencyShifter() {}
  void init(int32_t sampleRate);
  void frequency(float freq);
  // void amplitude(float n);
  /* Uses internal oscillators */
  // void process(float * buf, size_t size);
  void process(float *bufferLeft, float *bufferRight, size_t size);
  /* For use with external oscillators */
  void process(float *buf, float *bufUp, const float *cosw, const float *sinw,
               size_t size);

private:
  float phase_accumulator;
  float phase_increment;
  float magnitude;
  int32_t m_sampleRate;
  float delayed_i = 0;

  arm_biquad_casd_df1_inst_f32 i_ap;
  arm_biquad_casd_df1_inst_f32 q_ap;

  static const int n_stages = 4;

  float i_state[n_stages * 4];
  float q_state[n_stages * 4];

  const float c_coeffs_i[4] = {0.47944111608296202665, 0.87624358989504858020,
                               0.97660296916871658368, 0.99749940412203375040};
  const float c_coeffs_q[4] = {0.16177741706363166219, 0.73306690130335572242,
                               0.94536301966806279840, 0.99060051416704042460};

  float coeffs_i[n_stages * 5] = {c_coeffs_i[0], 0.f, -1.f, 0.f, c_coeffs_i[0],
                                  c_coeffs_i[1], 0.f, -1.f, 0.f, c_coeffs_i[1],
                                  c_coeffs_i[2], 0.f, -1.f, 0.f, c_coeffs_i[2],
                                  c_coeffs_i[3], 0.f, -1.f, 0.f, c_coeffs_i[3]};

  float coeffs_q[n_stages * 5] = {c_coeffs_q[0], 0.f, -1.f, 0.f, c_coeffs_q[0],
                                  c_coeffs_q[1], 0.f, -1.f, 0.f, c_coeffs_q[1],
                                  c_coeffs_q[2], 0.f, -1.f, 0.f, c_coeffs_q[2],
                                  c_coeffs_q[3], 0.f, -1.f, 0.f, c_coeffs_q[3]};

  float cosw[16];
  float sinw[16];
  float q[16];
};
} // namespace daisysp
