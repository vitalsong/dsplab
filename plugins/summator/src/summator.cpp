#include <summator.h>

#include <string>
#include <stdlib.h>
#include <math.h>

//предельное значение для буферизации,
//при превышении которого принимается решение о потере синхронизации
const int BUFFER_SIZE_LIMIT = 32767;

using namespace DspLab;

//-------------------------------------------------------------------------------------------------
Summator::Summator()
  : Device(2, 1)
{
    //диапазон форматов входных портов
    Device::setAvaliableInputFormats({Real, Complex}, 0);
    Device::setAvaliableInputFormats({Real, Complex}, 1);

    //диапазон форматов выходных портов
    Device::setAvaliableOutputFormats({Real, Complex});

    //текущие форматы по-умолчанию
    m_formats[0] = Real;
    m_formats[1] = Real;
}

//-------------------------------------------------------------------------------------------------
Summator::~Summator()
{
    Summator::stop();
}

//-------------------------------------------------------------------------------------------------
bool Summator::isInputDisabled(int nin)
{
    if (Device::input(nin)->connections().size() == 0) {
        return true;
    }

    if (Device::input(nin)->connections().at(0)->parent()->started() == false) {
        return true;
    }

    return false;
}

//-------------------------------------------------------------------------------------------------
void Summator::channelProcess(int nin)
{
    int iMainCh, iAuxCh;

    //подбор индексов
    if (nin == 0) {
        iMainCh = 0;
        iAuxCh = 1;
    } else {
        iMainCh = 1;
        iAuxCh = 0;
    }

    //чтение
    Device::ioRead(m_frame[iMainCh], iMainCh);

    //bypass, если второй канал неактивен
    if (isInputDisabled(iAuxCh)) {
        Device::ioWrite(m_frame[iMainCh]);
        return;
    }

    //анализ потери синхронизации (переполнение буфера)
    //ЗЫ: здесь должен быть нормализатор скорости!
    if ((m_in[iMainCh].size() > BUFFER_SIZE_LIMIT) && (m_in[iAuxCh].size() == 0)) {
        this->stop();
        return;
    }

    //если формат изменился
    if (m_frame[iMainCh].format() != m_formats[iMainCh]) {
        //меняем текущий формат
        m_formats[iMainCh] = m_frame[iMainCh].format();

        //если форматы несовместимы (нужно подумать)
        //...

        //очищаем буферы
        m_in[iMainCh].clear();
        m_in[iAuxCh].clear();
    }

    //буферизация
    buffering(iMainCh);

    //расчет суммы
    summation();

    //вывод
    writing();
}

//-------------------------------------------------------------------------------------------------
void Summator::buffering(int nin)
{
    if (m_formats[nin] == Real) {
        auto fdata = m_frame[nin].getReal();
        const real_t* data = fdata.data();
        int size = fdata.size();
        m_meta[nin] = fdata.meta();
        m_in[nin].write(data, size);
    } else {
        auto fdata = m_frame[nin].getComplex();
        const complex_t* data = fdata.data();
        int size = fdata.size();
        m_meta[nin] = fdata.meta();
        m_in[nin].write((const real_t*)data, (2 * size));
    }
}

//-------------------------------------------------------------------------------------------------
void Summator::summation()
{
    int size = std::min(m_in[0].size(), m_in[1].size());

    //проверка совместимости мета-информации
    //...
    //...
    //...

    if (size == 0) {
        m_out.clear();
        return;
    }

    m_out.resize(size);

    real_t* x1 = m_in[0].data();
    real_t* x2 = m_in[1].data();

    for (int i = 0; i < size; ++i) {
        m_out[i] = x1[i] + x2[i];
    }

    //очистка
    m_in[0].clear(size);
    m_in[1].clear(size);
}

//-------------------------------------------------------------------------------------------------
void Summator::writing()
{
    if (!m_out.size()) {
        return;
    }

    if (m_formats[0] == Real) {
        const real_t* pout = m_out.data();
        int nout = m_out.size();
        m_frameOut.setReal({pout, nout, m_meta[0]});
    } else {
        const complex_t* pout = (const complex_t*)m_out.data();
        int nout = m_out.size() / 2;
        m_frameOut.setComplex({pout, nout, m_meta[0]});
    }

    Device::ioWrite(m_frameOut);
}

//-------------------------------------------------------------------------------------------------
void Summator::processing()
{
    while ((Device::ioSize(0) > 0) || (Device::ioSize(1) > 0)) {
        //первый канал
        if (Device::ioSize(0) > 0) {
            this->channelProcess(0);
        }

        //второй канал
        if (Device::ioSize(1) > 0) {
            this->channelProcess(1);
        }
    }
}

//-------------------------------------------------------------------------------------------------
void Summator::start()
{
    Device::start();
}

//-------------------------------------------------------------------------------------------------
void Summator::stop()
{
    //очистка буферов
    m_in[0].clear();
    m_in[1].clear();

    //текущий формат
    m_formats[0] = Real;
    m_formats[1] = Real;

    Device::stop();
}
