
namespace dsp {

using UInt = uint16_t;

template <UInt S> class comb {
public:
  void init(const float ff = 0.f, const float fb = 0.f);
  inline float process(const float in);
  inline float tap(uint16_t in);
  inline void mod(float mod);
  inline void setDelayTime(const float delayTime);
  inline void update();

private:
  uint16_t m_size = S;
  float m_buf[S] {};
  uint16_t m_pos;
  float m_delayTime;
  float m_prev;

protected:
  float m_ff;
  float m_fb;
};

template <UInt S> void comb<S>::init(const float ff, const float fb) {
  m_pos = 0;
  m_ff = ff;
  m_fb = fb;
  m_delayTime = static_cast<float>(m_size);
}

template <UInt S> inline void comb<S>::setDelayTime(const float delayTime) {
  m_delayTime = delayTime;
}

template <UInt S> inline float comb<S>::process(const float in) {

  m_pos += 1;

  if (m_pos >= m_delayTime)
    m_pos -= m_delayTime;

  auto pos = static_cast<uint16_t>(m_pos);
  const float frac = m_pos - pos;

  float c = m_buf[pos % m_size];

  float buf_out = c;
  float y = in - buf_out * m_fb;
  m_buf[pos] = y;
  const float out = m_ff * y + buf_out;

  m_prev = out;

  return out;
}

template <UInt S> inline float comb<S>::tap(uint16_t in) {
  auto pos = m_pos;
  if (pos > in)
    pos -= in;
  return m_buf[pos];
}

} /* namespace dsp */
