#include "spectrum-process.h"

#define DEFAULT_NFFT (512)
#define DEFAULT_SCALE (0x7FFF)
#define DEFAULT_SAMPLE_RATE (8000)
#define DEFAULT_ACCUM_RATE (1)
#define DEFAULT_PEAKS_COUNT (3)

namespace Spectrum {

//-------------------------------------------------------------------------------------------------
Process::Process()
{
    m_nfft = DEFAULT_NFFT;
    m_scale = DEFAULT_SCALE;
    m_wind_mode = Hann;
    m_accum_rate = DEFAULT_ACCUM_RATE;
    m_accum_count = 0;
    m_sample_rate = DEFAULT_SAMPLE_RATE;
    m_fft_load = 0;
    m_breal = false;
    m_accum_mode = NotAccumulation;
    m_peaks_count = DEFAULT_PEAKS_COUNT;

    _refreshBuffers();
    _refreshWindow();
}

//-------------------------------------------------------------------------------------------------
void Process::setFftSize(int nfft)
{
    //округление к степени двойки (или без изменений)
    nfft = int(1) << dsplib::nextpow2(nfft);

    if ((nfft != m_nfft) && (nfft >= 32))
    {
        m_nfft = nfft;
        _refreshBuffers();
        _refreshWindow();
    }
}

//-------------------------------------------------------------------------------------------------
int Process::getFftSize() const
{
    return m_nfft;
}

//-------------------------------------------------------------------------------------------------
void Process::setFullScale(double scale)
{
    if (scale > 0) {
        m_scale = scale;
    }
}

//-------------------------------------------------------------------------------------------------
double Process::getFullScale() const
{
    return m_scale;
}

//-------------------------------------------------------------------------------------------------
void Process::setFftWindow(FftWindow wind)
{
    m_wind_mode = wind;
    _refreshWindow();
}

//-------------------------------------------------------------------------------------------------
FftWindow Process::getFftWindow() const
{
    return m_wind_mode;
}

//-------------------------------------------------------------------------------------------------
void Process::setAccumulationRate(int count)
{
    if (count > 0)
    {
        m_accum_rate = count;
        m_accum_count = 0;
        _refreshBuffers();
    }
}

//-------------------------------------------------------------------------------------------------
int Process::getAccumulationRate() const
{
    return m_accum_rate;
}

//-------------------------------------------------------------------------------------------------
void Process::setAccumulationMode(AccumulationMode mode)
{
    if (m_accum_mode != mode)
    {
        m_accum_mode = mode;
        _refreshBuffers();
    }
}

//-------------------------------------------------------------------------------------------------
AccumulationMode Process::getAccumulationMode() const
{
    return m_accum_mode;
}

//-------------------------------------------------------------------------------------------------
void Process::setSampleRate(qint64 sampleRate)
{
    if ((m_sample_rate != sampleRate) && (sampleRate > 1)) {
        m_sample_rate = sampleRate;
    }
}

//-------------------------------------------------------------------------------------------------
quint64 Process::getSampleRate() const
{
    return m_sample_rate;
}

//-------------------------------------------------------------------------------------------------
void Process::setPeaksCount(int count)
{
    m_peaks_count = count;
}

//-------------------------------------------------------------------------------------------------
int Process::getPeaksCount() const
{
    return m_peaks_count;
}

//-------------------------------------------------------------------------------------------------
void Process::write(const dsplib::arr_real &sig)
{
    //если прошлый набор был комплексным
    if (!m_breal)
    {
        m_breal = true;
        _refreshBuffers();
    }

    dsplib::arr_cmplx r = dsplib::complex(sig, dsplib::zeros(sig.size()));
    _writeToBuffer(r);
}

//-------------------------------------------------------------------------------------------------
void Process::write(const dsplib::arr_cmplx &sig)
{
    //если прошлый набор был реальным
    if (m_breal)
    {
        m_breal = false;
        _refreshBuffers();
    }

    _writeToBuffer(sig);
}

//-------------------------------------------------------------------------------------------------
void Process::reset()
{
    _refreshBuffers();
}

//-------------------------------------------------------------------------------------------------
void Process::_writeToBuffer(const dsplib::arr_cmplx &sig)
{
    int n = sig.size();
    for (int i=0; i < n; ++i)
    {
        //буферизация
        m_fft_buf[m_fft_load] = sig[i];
        m_fft_load += 1;

        //если накопился ровно БПФ период
        //расчет БПФ по текущему накопленному буферу
        if (m_fft_load == m_nfft)
        {
            m_fft_load = 0;
            _calculate();
        }
    }
}

//-------------------------------------------------------------------------------------------------
void Process::_refreshWindow()
{
    //перерасчет оконной функции
    switch (m_wind_mode)
    {
    case Rectangle:
        m_wind = dsplib::window::rectangle(m_nfft);
        m_cgain = 0;
        break;

    case Sinus:
        m_wind = dsplib::window::sinus(m_nfft);
        m_cgain = 3.93;
        break;

    case Hann:
        m_wind = dsplib::window::hann(m_nfft);
        m_cgain = 6;
        break;

    case Hamming:
        m_wind = dsplib::window::hamming(m_nfft);
        m_cgain = 6;
        break;

    case Blackman:
        m_wind = dsplib::window::blackman(m_nfft);
        m_cgain = 7.54;
        break;

    case Gauss:
        m_wind = dsplib::window::gauss(m_nfft);
        m_cgain = 8.52;
        break;
    }
}

//-------------------------------------------------------------------------------------------------
void Process::_refreshBuffers()
{
    m_fft_buf = dsplib::zeros(m_nfft);
    m_accum_amp = dsplib::zeros(m_nfft);
    m_fft_load = 0;
    m_accum_count = 0;

    //инициализация начальных значений для режимов min/max
    switch (m_accum_mode)
    {
    case AccumulationMax:
        m_accum_amp.slice(0, m_nfft) = 0;
        break;
    case AccumulationMin:
        m_accum_amp.slice(0, m_nfft) = m_scale * m_scale;
        break;

    default:
        break;
    }
}

//-------------------------------------------------------------------------------------------------
void Process::_calculate()
{
    dsplib::arr_cmplx& fft_in = m_fft_buf;

    //умножение на оконную функцию
    fft_in *= m_wind;

    //рассчитываем БПФ
    auto fft_out = dsplib::fft(fft_in);

    //нормализуем значения
    if (m_breal) {
        fft_out /= (m_nfft/2);
    }
    else {
        fft_out /= (m_nfft);
    }

    //расчет модуля
    auto fft_abs = dsplib::abs(fft_out);

    //накопление
    _updateFftAbs(fft_abs);
}

//-------------------------------------------------------------------------------------------------
void Process::_updateFftAbs(const dsplib::arr_real &fft_abs)
{
    switch (m_accum_mode)
    {
    case NotAccumulation:
        _accumOnce(fft_abs);
        break;

    case AccumulationMax:
        _accumMax(fft_abs);
        break;

    case AccumulationMin:
        _accumMin(fft_abs);
        break;

    case AccumulationMean:
        _accumMean(fft_abs);
        break;
    }
}

//-------------------------------------------------------------------------------------------------
void Process::_accumOnce(const dsplib::arr_real &fft_abs)
{
    m_accum_amp = fft_abs;

    //расчет в дБ
    m_accum_amp = dsplib::log10(m_accum_amp / m_scale) * 20 + m_cgain;

    //формирование спектра
    _formSpectrumPoints(m_accum_amp);
}

//-------------------------------------------------------------------------------------------------
void Process::_accumMean(const dsplib::arr_real &fft_abs)
{
    //накопление
    m_accum_amp += fft_abs;
    m_accum_count += 1;

    //если накоплено достаточно
    if (m_accum_count == m_accum_rate)
    {
        //расчет в дБ
        m_accum_amp /= m_accum_count;
        m_accum_amp = dsplib::log10(m_accum_amp / m_scale) * 20 + m_cgain;

        //формирование спектра
        _formSpectrumPoints(m_accum_amp);

        //сброс счетчика
        m_accum_count = 0;

        //обнуление накопителя
        m_accum_amp = dsplib::zeros(m_nfft);
    }
}

//-------------------------------------------------------------------------------------------------
void Process::_accumMax(const dsplib::arr_real &fft_abs)
{
    int n = fft_abs.size();
    for (int i=0; i < n; ++i)
    {
        if (m_accum_amp[i] < fft_abs[i]) {
            m_accum_amp[i] = fft_abs[i];
        }
    }

    auto spectrum = dsplib::log10(m_accum_amp / m_scale) * 20 + m_cgain;

    _formSpectrumPoints(spectrum);
}

//-------------------------------------------------------------------------------------------------
void Process::_accumMin(const dsplib::arr_real &fft_abs)
{
    int n = fft_abs.size();
    for (int i=0; i < n; ++i)
    {
        if (m_accum_amp[i] > fft_abs[i]) {
            m_accum_amp[i] = fft_abs[i];
        }
    }

    auto spectrum = dsplib::log10(m_accum_amp / m_scale) * 20 + m_cgain;

    _formSpectrumPoints(spectrum);
}

//-------------------------------------------------------------------------------------------------
//формирование набора точек для реального сигнала
static QVector <QPointF> _formRealPoints(const dsplib::arr_real &spectrum, int fs)
{
    int n = spectrum.size();
    QVector <QPointF> r(n/2);
    double dt = double(fs) / n;

    double rx = 0;
    for (int i=0; i < n/2; ++i)
    {
        r[i] = QPointF(rx, spectrum[i]);
        rx += dt;
    }

    return r;
}

//-------------------------------------------------------------------------------------------------
//формирование набора точек для реального сигнала
static QVector <QPointF> _formComplexPoints(const dsplib::arr_real &spectrum, int fs)
{
    int n = spectrum.size();
    QVector <QPointF> r(n);
    double dt = double(fs) / n;

    int m = 0;
    double rx = (-1) * fs/2;
    for (int i=0; i < n; ++i)
    {
        //начинаем с середины выборки
        m = (i + n/2) % n;
        r[i] = QPointF(rx, spectrum[m]);
        rx += dt;
    }

    return r;
}

//-------------------------------------------------------------------------------------------------
void Process::_formSpectrumPoints(const dsplib::arr_real &spectrum)
{
    if (m_breal)
    {
        auto points = _formRealPoints(spectrum, m_sample_rate);
        emit spectrumReady(points);
    }
    else
    {
        auto points = _formComplexPoints(spectrum, m_sample_rate);
        emit spectrumReady(points);
    }

    //формирование списка пиков
    _formPeaksPoints(spectrum);
}

//-------------------------------------------------------------------------------------------------
//поиск пиковой частоты и обнуление всех точек по склонам (для следующего этапа поиска)
static QPointF _findAndClearPeak(QVector <QPointF>& points)
{
    //поиск пика
    double max = -1000;
    int imax = 0;
    int n = points.size();
    for (int i=0; i < n; ++i)
    {
        if (points[i].y() > max)
        {
            max = points[i].y();
            imax = i;
        }
    }

    QPointF result = points[imax];

    //проход по левому склону
    int left_pos = imax;
    while ((left_pos != 0) && (points[left_pos].y() - points[left_pos-1].y() >= 0)) {
        --left_pos;
    }

    //проход по правому склону
    int right_pos = imax;
    while ((right_pos < n-1) && (points[right_pos].y() - points[right_pos+1].y() >= 0)) {
        ++right_pos;
    }

    //очистка
    for (int i=left_pos; i <= right_pos; ++i) {
        points[i].ry() = -1000;
    }

    return result;
}

//-------------------------------------------------------------------------------------------------
void Process::_formPeaksPoints(const dsplib::arr_real &spectrum)
{
    if (m_peaks_count == 0) {
        return;
    }

    QVector <QPointF> spectrum_points;
    QVector <QPointF> peak_points;

    //получение точек спектра
    if (m_breal) {
        spectrum_points = _formRealPoints(spectrum, m_sample_rate);
    }
    else {
        spectrum_points = _formComplexPoints(spectrum, m_sample_rate);
    }

    //формирование списка пиковых гармоник
    for (int i=0; i < m_peaks_count; ++i) {
        peak_points.push_back(_findAndClearPeak(spectrum_points));
    }

    emit peaksReady(peak_points);
}

} /// < Spectrum
