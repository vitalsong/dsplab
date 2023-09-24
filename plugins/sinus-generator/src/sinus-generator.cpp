#include "sinus-generator.h"

#include <thread>
#include <qmath.h>
#include <unistd.h>

using namespace DspLab;

//---------------------------------------------------------------
SinusGenerator::SinusGenerator()
  : Device(0, 1)
  , m_ampPerDb(0)
  , m_fscale(32767)
  , m_sampleRate(8000)
  , m_frequency(440)
  , m_ampPerScale(32767)
{
    //настройка выходного порта по-умолчанию
    Device::setAvaliableOutputFormats({Real});

    //настройка периода генерации по-умолчанию
    this->setGeneratePeriod(20);
}

//---------------------------------------------------------------
SinusGenerator::~SinusGenerator()
{
    SinusGenerator::stop();
}

//---------------------------------------------------------------
void SinusGenerator::start()
{
    //перерасчет количества сэмплов для генерации одного пакета
    //ЗЫ: некратность на данный момент не учитывается!
    int nsmp = double(m_sampleRate) * double(m_period) / 1000;
    m_buffer.resize(nsmp);

    //индекс начальной фазы равен 0
    m_iphase = 0;

    Device::start();
}

//---------------------------------------------------------------
void SinusGenerator::stop()
{
    Device::stop();
}

//---------------------------------------------------------------
void SinusGenerator::processing()
{
    int fs = m_sampleRate;
    int amp = m_ampPerScale;
    int freq = m_frequency;
    int& iphase = m_iphase;

    real_t* x = m_buffer.data();
    int nx = m_buffer.size();

    //генерация
    double phase;
    for (int i = 0; i < nx; ++i) {
        phase = 2 * M_PI * freq * (double(iphase) / double(fs));
        iphase = (iphase + 1) % fs;
        x[i] = amp * sin(phase);
    }

    //запаковка
    DspMeta meta(m_sampleRate, (m_sampleRate / 2), m_fscale, 1);
    m_frame.setReal({x, nx, meta});

    //отправка по выходу
    Device::ioWrite(m_frame);
}

//---------------------------------------------------------------
void SinusGenerator::setSampleRateHz(int value)
{
    if (!this->started()) {
        m_sampleRate = value;

        //теорема Котельникова
        if (m_frequency > m_sampleRate / 2) {
            this->setFrequensyHz(m_sampleRate / 2);
        }
    }
}

//---------------------------------------------------------------
int SinusGenerator::getSampleRateHz() const
{
    return m_sampleRate;
}

//---------------------------------------------------------------
void SinusGenerator::setAmplitudeDB(double value)
{
    m_ampPerDb = value;
    m_ampPerScale = round(pow(10, (m_ampPerDb / 20.0f) + log10(m_fscale)));
}

//---------------------------------------------------------------
double SinusGenerator::getAmplitudeDB() const
{
    return m_ampPerDb;
}

//---------------------------------------------------------------
void SinusGenerator::setFullScale(double value)
{
    if (!this->started()) {
        m_fscale = value;
        m_ampPerScale = round(pow(10, (m_ampPerDb / 20.0f) + log10(m_fscale)));
    }
}

//---------------------------------------------------------------
double SinusGenerator::getFullScale() const
{
    return m_fscale;
}

//---------------------------------------------------------------
void SinusGenerator::setFrequensyHz(int value)
{
    //теорема Котельникова
    if (value <= m_sampleRate / 2) {
        m_frequency = value;
    }
}

//---------------------------------------------------------------
int SinusGenerator::getFrequensyHz() const
{
    return m_frequency;
}

//---------------------------------------------------------------
void SinusGenerator::setGeneratePeriod(int value)
{
    if (!this->started()) {
        m_period = value;
        Device::setProcessingTimeoutInterval(value);
    }
}

//---------------------------------------------------------------
int SinusGenerator::getGeneratePeriod() const
{
    return m_period;
}
