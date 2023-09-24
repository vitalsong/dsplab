#include "transformer-fb.h"

#include <string.h>
#include <stdlib.h>
#include <assert.h>

using namespace DspLab;

//-------------------------------------------------------------------------------------------------
TransformerFB::TransformerFB()
  : Device(1, 1)
{
    //настройка допустимых форматов для входного порта
    Device::setAvaliableInputFormats({{Special, Raw}});

    //форматы выхода
    Device::setAvaliableOutputFormats({Real, Complex});

    //настройка параметров по умолчанию
    this->setMidFormat(InInt16);
    this->setOutFormat(OutReal);
}

//-------------------------------------------------------------------------------------------------
TransformerFB::~TransformerFB()
{
    TransformerFB::stop();
}

//-------------------------------------------------------------------------------------------------
void TransformerFB::processing()
{
    while (Device::ioSize() > 0) {
        //чтение из порта
        Device::ioRead(m_inFrame);

        //распаковка фрейма
        auto package = m_inFrame.getSpecial();
        const char* data = package.data();
        int size = package.size();

        //накопление
        for (int i = 0; i < size; ++i) {
            m_midBuffer.push_back(data[i]);
        }

        //распаковка из байтового формата
        this->convertDataToInt();

        //запаковка выходного фрейма и отправка
        if (m_outFormat == OutReal) {
            //действительный выход
            const real_t* pout = m_outBuffer.data();
            int nout = m_outBuffer.size();
            m_outFrame.setReal({pout, nout});
        } else {
            //комплексный выход
            const complex_t* pout = (const complex_t*)m_outBuffer.data();
            int nout = m_outBuffer.size() / 2;
            m_outFrame.setComplex({pout, nout});
        }

        //отправляем фрейм
        Device::ioWrite(m_outFrame);
    }
}

//-------------------------------------------------------------------------------------------------
void TransformerFB::setMidFormat(TransformerFB::MidFormat format)
{
    m_midFormat = format;
}

//-------------------------------------------------------------------------------------------------
TransformerFB::MidFormat TransformerFB::getMidFormat() const
{
    return m_midFormat;
}

//-------------------------------------------------------------------------------------------------
void TransformerFB::setOutFormat(OutFormat format)
{
    m_outFormat = format;
}

//-------------------------------------------------------------------------------------------------
TransformerFB::OutFormat TransformerFB::getOutFormat() const
{
    return m_outFormat;
}

//-------------------------------------------------------------------------------------------------
static int sConvertToInt8(const char* in, int nin, real_t* out, int nout)
{
    assert(nout >= nin);

    for (int i = 0; i < nin; ++i) {
        out[i] = in[i];
    }

    return nin;
}

//-------------------------------------------------------------------------------------------------
static int sConvertToInt16(const char* in, int nin, real_t* out, int nout)
{
    int nx;
    nx = nin / 2;

    assert(nin % 2 == 0);
    assert(nout >= nx);

    short* ptr = (short*)in;
    for (int i = 0; i < nx; ++i) {
        out[i] = ptr[i];
    }

    return nx;
}

//-------------------------------------------------------------------------------------------------
static int sConvertToInt24(const char* in, int nin, real_t* out, int nout)
{
    int nx;
    nx = nin / 3;

    assert(nin % 3 == 0);
    assert(nout >= nx);

    unsigned char* ptr = (unsigned char*)in;
    for (int i = 0; i < nx; ++i) {
        int value = 0;
        value |= ((unsigned int)ptr[0]) << 24;
        value |= ((unsigned int)ptr[1]) << 16;
        value |= ((unsigned int)ptr[2]) << 8;
        out[i] = (value >> 8);
        ptr += 3;
    }

    return nx;
}

//-------------------------------------------------------------------------------------------------
static int sConvertToInt32(const char* in, int nin, real_t* out, int nout)
{
    int nx;
    nx = nin / 4;

    assert(nin % 4 == 0);
    assert(nout >= nx);

    int* ptr = (int*)in;
    for (int i = 0; i < nx; ++i) {
        out[i] = ptr[i];
    }

    return nx;
}

//-------------------------------------------------------------------------------------------------
static int sConvertToInt40(const char* in, int nin, real_t* out, int nout)
{
    int nx;
    nx = nin / 5;

    assert(nin % 5 == 0);
    assert(nout >= nx);

    unsigned char* ptr = (unsigned char*)in;
    for (int i = 0; i < nx; ++i) {
        long long int value = 0;
        value |= ((unsigned long long int)ptr[0]) << 56;
        value |= ((unsigned long long int)ptr[1]) << 48;
        value |= ((unsigned long long int)ptr[2]) << 40;
        value |= ((unsigned long long int)ptr[3]) << 32;
        value |= ((unsigned long long int)ptr[4]) << 24;
        out[i] = (value >> 24);
        ptr += 5;
    }

    return nx;
}

//-------------------------------------------------------------------------------------------------
void TransformerFB::convertDataToInt()
{
    int nsmp, nbyte;

    switch (m_midFormat) {
    case InInt8: {
        nsmp = (m_midBuffer.size() / 2) * 2;
        nbyte = nsmp;
        m_outBuffer.resize(nsmp);
        sConvertToInt8(m_midBuffer.data(), nbyte, m_outBuffer.data(), nsmp);
        break;
    }

    case InInt16: {
        const int INT16_PER_CHAR = 2;
        nsmp = ((m_midBuffer.size() / INT16_PER_CHAR) / 2) * 2;
        nbyte = nsmp * INT16_PER_CHAR;
        m_outBuffer.resize(nsmp);
        sConvertToInt16(m_midBuffer.data(), nbyte, m_outBuffer.data(), nsmp);
        break;
    }

    case InInt24: {
        const int INT24_PER_CHAR = 3;
        nsmp = ((m_midBuffer.size() / INT24_PER_CHAR) / 2) * 2;
        nbyte = nsmp * INT24_PER_CHAR;
        m_outBuffer.resize(nsmp);
        sConvertToInt24(m_midBuffer.data(), nbyte, m_outBuffer.data(), nsmp);
        break;
    }

    case InInt32: {
        const int INT32_PER_CHAR = 4;
        nsmp = ((m_midBuffer.size() / INT32_PER_CHAR) / 2) * 2;
        nbyte = nsmp * INT32_PER_CHAR;
        m_outBuffer.resize(nsmp);
        sConvertToInt32(m_midBuffer.data(), nbyte, m_outBuffer.data(), nsmp);
        break;
    }

    case InInt40: {
        const int INT40PER_CHAR = 5;
        nsmp = ((m_midBuffer.size() / INT40PER_CHAR) / 2) * 2;
        nbyte = nsmp * INT40PER_CHAR;
        m_outBuffer.resize(nsmp);
        sConvertToInt40(m_midBuffer.data(), nbyte, m_outBuffer.data(), nsmp);
        break;
    }

    default:
        return;
    }

    //хранение остатка
    memmove(m_midBuffer.data(), m_midBuffer.data() + nbyte, (m_midBuffer.size() - nbyte));
    m_midBuffer.resize(m_midBuffer.size() - nbyte);
}
