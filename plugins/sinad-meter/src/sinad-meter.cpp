#include <sinad-meter.h>
#include <dsplib.h>

//-------------------------------------------------------------------------------------------------
SinadMeter::SinadMeter(QObject* parent)
  : QObject(parent)
{
    std::lock_guard<std::mutex> locker(m_mutex);

    m_sinad = 0;
    m_nfft = 1024;
    m_buffer.resize(m_nfft);
    m_spectrum.resize(m_nfft / 2);
    m_buffer_load = 0;
    m_fixed_freq = false;
    m_freq_num = 0;
    m_accum_period = 16;
    m_accum_count = 0;
}

//-------------------------------------------------------------------------------------------------
void SinadMeter::setFftSize(int nfft)
{
    std::lock_guard<std::mutex> locker(m_mutex);

    m_nfft = nfft;
    m_buffer.resize(m_nfft);
    m_spectrum.resize(m_nfft / 2);
    m_buffer_load = 0;
}

//-------------------------------------------------------------------------------------------------
int SinadMeter::fftSize() const
{
    return m_nfft;
}

//-------------------------------------------------------------------------------------------------
void SinadMeter::setFftAccumulation(int count)
{
    std::lock_guard<std::mutex> locker(m_mutex);

    m_accum_period = count;
    m_accum_count = 0;
}

//-------------------------------------------------------------------------------------------------
int SinadMeter::fftAccumulation() const
{
    return m_accum_period;
}

//-------------------------------------------------------------------------------------------------
void SinadMeter::setFixedMode(bool enabled)
{
    std::lock_guard<std::mutex> locker(m_mutex);

    m_fixed_freq = enabled;
    m_freq_num = 0;
}

//-------------------------------------------------------------------------------------------------
bool SinadMeter::isFixedMode() const
{
    return m_fixed_freq;
}

//-------------------------------------------------------------------------------------------------
void SinadMeter::setFixedPeakBin(int ibin)
{
    std::lock_guard<std::mutex> locker(m_mutex);

    if (m_fixed_freq) {
        m_freq_num = ibin;
    }
}

//-------------------------------------------------------------------------------------------------
int SinadMeter::fixedPeakBin() const
{
    std::lock_guard<std::mutex> locker(m_mutex);
    return m_freq_num;
}

//-------------------------------------------------------------------------------------------------
double SinadMeter::value() const
{
    std::lock_guard<std::mutex> locker(m_mutex);
    return m_sinad;
}

//-------------------------------------------------------------------------------------------------
void SinadMeter::process(const double* x, int nx)
{
    std::lock_guard<std::mutex> locker(m_mutex);

    for (int i = 0; i < nx; ++i) {
        //накопление
        m_buffer[m_buffer_load] = x[i];
        m_buffer_load += 1;

        //когда буфер готов для новых расчетов
        if (m_buffer_load == m_nfft) {
            //сброс счетчика
            m_buffer_load = 0;

            //расчет спектра (накопление спектра)
            _calculateSpectrum();

            //если накоплено достаточно БПФ выборок
            if (m_accum_count == m_accum_period) {
                //сброс счетчика
                m_accum_count = 0;

                //расчет нового значения sinad
                _calculateSinad();

                //обнуление буфера
                memset(m_spectrum.data(), 0, m_spectrum.size() * sizeof(double));

                //генерация сигнала с результатом
                emit valueChanged(m_sinad);
            }
        }
    }
}

//-------------------------------------------------------------------------------------------------
void SinadMeter::reset()
{
    std::lock_guard<std::mutex> locker(m_mutex);

    m_buffer_load = 0;
    m_accum_count = 0;
    m_sinad = 0;
}

//-------------------------------------------------------------------------------------------------
void SinadMeter::_calculateSpectrum()
{
    //умножение на оконную функцию
    auto x = dsplib::arr_real(m_buffer.data(), m_nfft);
    auto fft_in = x * dsplib::window::gauss(m_nfft);

    //расчет БПФ
    auto fft_out = dsplib::fft(fft_in);

    //расчет модуля c накоплением
    double re, im;
    for (int i = 0; i < m_nfft / 2; ++i) {
        re = fft_out[i].re;
        im = fft_out[i].im;
        m_spectrum[i] += ((re * re) + (im * im)) / m_accum_period;
    }

    //обновление счетчика накопления
    m_accum_count += 1;
}

//-------------------------------------------------------------------------------------------------
void SinadMeter::_calculateSinad()
{
    int left_pos, right_pos;
    double pow_signal, pow_noise;

    //поиск пика
    if (!m_fixed_freq) {
        m_freq_num = dsplib::argmax(dsplib::arr_real(m_spectrum.data(), m_nfft / 2));
    }

    //оценка мощности сигнала
    left_pos = m_freq_num;
    while ((left_pos != 0) && (m_spectrum[left_pos] - m_spectrum[left_pos - 1] > 0)) {
        --left_pos;
    }

    right_pos = m_freq_num;
    while ((right_pos != m_nfft / 2 - 1) && (m_spectrum[right_pos] - m_spectrum[right_pos + 1] > 0)) {
        ++right_pos;
    }

    pow_signal = 0;
    for (int i = left_pos; i < right_pos + 1; ++i) {
        pow_signal += m_spectrum[i];
    }

    //оценка мощности шума
    pow_noise = 0;
    for (int i = 0; i < m_nfft / 2; ++i) {
        pow_noise += m_spectrum[i];
    }
    pow_noise -= pow_signal;

    //расчет sinad
    m_sinad = 10 * log10(pow_signal / (pow_noise + 0.01));
}
