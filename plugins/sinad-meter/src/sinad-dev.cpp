#include <sinad-dev.h>
#include <sinad-meter.h>

using namespace DspLab;

//-------------------------------------------------------------------------------------------------
SinadDev::SinadDev()
  : Device(1, 0)
{
    //настройка допустимых форматов для входного порта
    Device::setAvaliableInputFormats({Real});
    m_meter = new SinadMeter(this);
    m_meter->setFftSize(4096);
    m_meter->setFftAccumulation(8);

    connect(m_meter, SIGNAL(valueChanged(double)), this, SLOT(onSinadValueChanged(double)));
}

//-------------------------------------------------------------------------------------------------
SinadDev::~SinadDev()
{
    SinadDev::stop();
}

//-------------------------------------------------------------------------------------------------
void SinadDev::processing()
{
    while (Device::ioSize() > 0) {
        //чтение из порта
        Device::ioRead(m_frame);
        auto package = m_frame.getReal();
        const real_t* data = package.data();
        int size = package.size();

        //расчет SINAD
        m_meter->process(data, size);
    }
}

//-------------------------------------------------------------------------------------------------
double SinadDev::value() const
{
    return m_value;
}

//-------------------------------------------------------------------------------------------------
void SinadDev::onSinadValueChanged(double value)
{
    m_value = value;
}
