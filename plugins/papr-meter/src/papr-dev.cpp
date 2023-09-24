#include "papr-dev.h"

#include <string.h>

#define DEFAULT_MEASURE_PERIOD (8000)

using namespace DspLab;

//-------------------------------------------------------------------------------------------------
PaprDev::PaprDev()
  : Device(1, 0)
{
    _resetParams();
    m_papr = 0;
    m_period = DEFAULT_MEASURE_PERIOD;
    Device::setAvaliableInputFormats({Real});
}

//-------------------------------------------------------------------------------------------------
PaprDev::~PaprDev()
{
    PaprDev::stop();
}

//-------------------------------------------------------------------------------------------------
void PaprDev::_update(const double* x, int nx)
{
    for (int i = 0; i < nx; ++i) {
        m_rms += (x[i] * x[i]) / m_period;
        m_max = (fabs(x[i]) > m_max) ? (fabs(x[i])) : (m_max);
        m_counter += 1;

        if (m_counter == m_period) {
            m_papr = 10 * log10((m_max * m_max) / m_rms);
            _resetParams();
        }
    }
}

//-------------------------------------------------------------------------------------------------
void PaprDev::_resetParams()
{
    m_rms = 0;
    m_max = 0;
    m_counter = 0;
}

//-------------------------------------------------------------------------------------------------
void PaprDev::processing()
{
    while (Device::ioSize() > 0) {
        Device::ioRead(m_frame);
        auto package = m_frame.getReal();
        const real_t* data = package.data();
        int size = package.size();
        _update(data, size);
    }
}

//-------------------------------------------------------------------------------------------------
double PaprDev::value() const
{
    return m_papr;
}

//-------------------------------------------------------------------------------------------------
void PaprDev::setMeasurePeriod(int nsmp)
{
    if (nsmp > 0) {
        _resetParams();
        m_period = nsmp;
    }
}

//-------------------------------------------------------------------------------------------------
int PaprDev::measurePeriod()
{
    return m_period;
}
