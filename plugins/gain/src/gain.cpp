#include "gain.h"

#include <memory>
#include <math.h>

using namespace DspLab;

//-------------------------------------------------------------------------------------------------
Gain::Gain()
  : Device(1, 1)
{
    //настройка доступных форматов
    Device::setAvaliableInputFormats({Real, Complex});
    Device::setAvaliableOutputFormats({Real, Complex});

    //настройки по-умолчанию
    m_gain = 0;
    m_mlt = 1;
}

//-------------------------------------------------------------------------------------------------
Gain::~Gain()
{
    Gain::stop();
}

//-------------------------------------------------------------------------------------------------
void Gain::processing()
{
    while (Device::ioSize() > 0) {
        Device::ioRead(m_frameIn);

        if (m_frameIn.format() == Complex) {
            auto package = m_frameIn.getComplex();
            const complex_t* data = package.data();
            auto meta = package.meta();
            int size = package.size();
            m_buffer.resize(size * 2);

            complex_t* buffer = (complex_t*)m_buffer.data();
            for (int i = 0; i < size; ++i) {
                buffer[i].xi = (data[i].xi * m_mlt);
                buffer[i].xq = (data[i].xq * m_mlt);
            }

            m_frameOut.setComplex({buffer, size, meta});
            Device::ioWrite(m_frameOut);
        } else {
            auto package = m_frameIn.getReal();
            const real_t* data = package.data();
            auto meta = package.meta();
            int size = package.size();
            m_buffer.resize(size);

            for (int i = 0; i < size; ++i) {
                m_buffer[i] = data[i] * m_mlt;
            }

            m_frameOut.setReal({m_buffer.data(), size, meta});
            Device::ioWrite(m_frameOut);
        }
    }
}

//-------------------------------------------------------------------------------------------------
void Gain::changeGain(double value)
{
    //расчет множителя
    m_gain = value;
    m_mlt = pow(10, m_gain / 20);
}

//-------------------------------------------------------------------------------------------------
double Gain::currentGain() const
{
    return m_gain;
}
