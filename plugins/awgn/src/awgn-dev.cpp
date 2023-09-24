#include <awgn-dev.h>

#include <stdlib.h>
#include <math.h>

using namespace DspLab;

//-------------------------------------------------------------------------------------------------
AwgnDev::AwgnDev()
  : Device(1, 1)
{
    Device::setAvaliableInputFormats({Real, Complex});
    Device::setAvaliableOutputFormats({Real, Complex});

    m_snr = 20;
    m_smth = 0.01;
    m_rms = 0;
    m_rmsxq = 0;
    m_expect = 0;
}

//-------------------------------------------------------------------------------------------------
AwgnDev::~AwgnDev()
{
    AwgnDev::stop();
}

//-------------------------------------------------------------------------------------------------
static double _awgn(double expectation)
{
    double value;
    const int M = 12;
    double sum = 0;
    for (int j = 0; j < M; ++j) {
        value = rand();
        sum += value / RAND_MAX;
    }

    return (expectation) * (sum - M / 2);
}

//-------------------------------------------------------------------------------------------------
void AwgnDev::processing()
{
    while (Device::ioSize() > 0) {
        //чтение из порта
        Device::ioRead(m_inframe);

        //если формат фрейма изменился
        if (m_inframe.format() == Real) {
            _processReal(m_inframe);
        } else {
            _processComplex(m_inframe);
        }
    }
}

//-------------------------------------------------------------------------------------------------
void AwgnDev::_processReal(const Frame& frame)
{
    //распаковка фрейма
    auto package = frame.getReal();
    const real_t* data = package.data();
    DspMeta meta = package.meta();
    int size = package.size();
    m_real.resize(size);

    for (int i = 0; i < size; ++i) {
        //обновляем текущее значение rms
        m_rms = m_rms * (1 - m_smth) + (data[i] * data[i]) * m_smth;

        //перерасчитываем мощность шума
        m_expect = sqrt(m_rms / pow(10, m_snr / 10));

        //генерируем один отсчет с шумовой компонентой и подмешиваем компонент к сигналу
        m_real[i] = data[i] + _awgn(m_expect);
    }

    //запаковка выходного фрейма и отправка
    m_outframe.setReal({m_real.data(), size, meta});

    //отправляем фрейм
    Device::ioWrite(m_outframe);
}

//-------------------------------------------------------------------------------------------------
void AwgnDev::_processComplex(const Frame& frame)
{
    //распаковка фрейма
    auto package = frame.getComplex();
    const complex_t* data = package.data();
    DspMeta meta = package.meta();
    int size = package.size();
    m_real.resize(2 * size);

    for (int i = 0; i < size; ++i) {
        double absCmplx = (data[i].xi * data[i].xi) + (data[i].xq * data[i].xq);

        //обновляем текущее значение rms
        m_rms = m_rms * (1 - m_smth) + absCmplx * m_smth;

        //перерасчитываем мощность шума
        m_expect = sqrt(m_rms / pow(10, m_snr / 10));

        //генерируем один отсчет с шумовой компонентой и подмешиваем компонент к сигналу
        m_real[2 * i] = data[i].xi + _awgn(m_expect);
        m_real[2 * i + 1] = data[i].xq + _awgn(m_expect);
    }

    //запаковка выходного фрейма и отправка
    m_outframe.setComplex({(const complex_t*)m_real.data(), size, meta});

    //отправляем фрейм
    Device::ioWrite(m_outframe);
}

//-------------------------------------------------------------------------------------------------
void AwgnDev::setSnr(double snr)
{
    m_snr = snr;
}

//-------------------------------------------------------------------------------------------------
double AwgnDev::getSnr()
{
    return m_snr;
}

//-------------------------------------------------------------------------------------------------
void DspLab::AwgnDev::setExpCoeff(double alpha)
{
    m_smth = alpha;
}

//-------------------------------------------------------------------------------------------------
double AwgnDev::getExpCoeff()
{
    return m_smth;
}
