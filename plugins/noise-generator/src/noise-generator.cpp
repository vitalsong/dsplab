#include <noise-generator.h>

using namespace DspLab;

//-------------------------------------------------------------------------------------------------
NoiseGenerator::NoiseGenerator()
  : Device(0, 1)
  , m_bcomplex(false)
  , m_period(20)
  , m_sampleRate(8000)
  , m_sampleCount(100)
  , m_expectation(0)
  , m_deviation(10.0f)
{
    //настройка портов
    Device::setAvaliableOutputFormats({Real, Complex});

    //настройка периода генерации (по-умолчанию)
    this->setGeneratePeriod(m_period);
}

//-------------------------------------------------------------------------------------------------
NoiseGenerator::~NoiseGenerator()
{
    std::lock_guard<std::mutex> locker(m_mutex);
    NoiseGenerator::stop();
}

//-------------------------------------------------------------------------------------------------
void NoiseGenerator::processing()
{
    std::lock_guard<std::mutex> locker(m_mutex);

    if (m_bcomplex) {
        generateNoise(m_buffer.data(), (m_sampleCount * 2), m_expectation, m_deviation);
        DspMeta meta(m_sampleRate, 0, 32767, 1);
        m_frame.setComplex({(complex_t*)m_buffer.data(), m_sampleCount, meta});
    } else {
        generateNoise(m_buffer.data(), m_sampleCount, m_expectation, m_deviation);
        DspMeta meta(m_sampleRate, 0, 32767, 1);
        m_frame.setReal({m_buffer.data(), m_sampleCount, meta});
    }

    //отправляем данные
    Device::ioWrite(m_frame);
}

//-------------------------------------------------------------------------------------------------
void NoiseGenerator::setComplexTypeEnabled(bool enabled)
{
    std::lock_guard<std::mutex> locker(m_mutex);
    m_bcomplex = enabled;
}

//-------------------------------------------------------------------------------------------------
bool NoiseGenerator::isComplexTypeEnabled() const
{
    return m_bcomplex;
}

//-------------------------------------------------------------------------------------------------
void NoiseGenerator::setGeneratePeriod(int msec)
{
    std::lock_guard<std::mutex> locker(m_mutex);

    if (this->started() || msec <= 0) {
        return;
    }

    //настройка размера буфера под сигнал
    m_period = msec;
    m_sampleCount = (m_sampleRate * m_period) / 1000.0 + 0.5;
    m_buffer.resize(m_sampleCount * 2);

    //настройка таймера для инициативной генерации
    Device::setProcessingTimeoutInterval(m_period);
}

//-------------------------------------------------------------------------------------------------
int NoiseGenerator::generatePeriod() const
{
    return m_period;
}

//-------------------------------------------------------------------------------------------------
void NoiseGenerator::setSampleRate(int value)
{
    std::lock_guard<std::mutex> locker(m_mutex);

    if (this->started() || value <= 0) {
        return;
    }

    //настройка размера буфера под сигнал
    m_sampleRate = value;
    m_sampleCount = (m_sampleRate * m_period) / 1000.0 + 0.5;
    m_buffer.resize(m_sampleCount);
}

//-------------------------------------------------------------------------------------------------
int NoiseGenerator::sampleRate() const
{
    return m_sampleRate;
}

//-------------------------------------------------------------------------------------------------
void NoiseGenerator::setExpectation(double value)
{
    std::lock_guard<std::mutex> locker(m_mutex);

    if (!this->started()) {
        m_expectation = value;
    }
}

//-------------------------------------------------------------------------------------------------
double NoiseGenerator::expectation() const
{
    return m_expectation;
}

//-------------------------------------------------------------------------------------------------
void NoiseGenerator::setDeviation(double value)
{
    std::lock_guard<std::mutex> locker(m_mutex);

    if (!this->started()) {
        m_deviation = value;
    }
}

//-------------------------------------------------------------------------------------------------
double NoiseGenerator::deviation() const
{
    return m_deviation;
}

//-------------------------------------------------------------------------------------------------
void NoiseGenerator::generateNoise(real_t* x, int nx, double m, double q)
{
    //Лайонс Р. - 13.11 Генерация нормально распределенных случайных сигналов
    double sum;
    for (int i = 0; i < nx; ++i) {
        sum = 0;
        for (int j = 0; j < 12; ++j) {
            sum += rand();
        }

        sum /= RAND_MAX;
        x[i] = q * (sum - 6) + m;
    }
}
