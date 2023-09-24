#include "transform-tb.h"
#include "dsplab/format.h"

#include <string>
#include <stdlib.h>
#include <math.h>
#include <assert.h>

using namespace DspLab;

//----------------------------------------------------------------------------------------------
TransformTB::TransformTB()
  : Device(1, 1)
{
    //настройка допустимых форматов для входного порта
    Device::setAvaliableInputFormats({Real, Complex});

    //настройка допустимых форматов для выходного порта
    Device::setAvaliableOutputFormats({{Special, Raw}});

    //настройка промежуточного формата по-умолчанию
    this->setIntegerFormat(MID_INT16);
}

//----------------------------------------------------------------------------------------------
TransformTB::~TransformTB()
{
    TransformTB::stop();
}

//----------------------------------------------------------------------------------------------
void TransformTB::processing()
{
    while (Device::ioSize() > 0) {
        //чтение из порта
        Device::ioRead(m_inframe);

        //преобразование типа
        if (m_inframe.format() == Real) {
            //распаковка фрейма
            auto package = m_inframe.getReal();
            const real_t* data = package.data();
            int size = package.size();

            //запаковка в байтовый формат
            m_buffer.resize(size * 5);
            int nres = convert(data, size, m_buffer.data(), m_buffer.size(), m_format);

            //запаковка выходного фрейма
            m_outframe.setSpecial({(char*)m_buffer.data(), nres, Raw});
        } else if (m_inframe.format() == Complex) {
            //распаковка фрейма
            auto package = m_inframe.getComplex();
            const complex_t* data = package.data();
            int size = package.size();

            //запаковка в промежуточный байтовый формат
            m_buffer.resize(size * 5 * 2);
            int nres = convert((real_t*)data, (2 * size), m_buffer.data(), m_buffer.size(), m_format);

            //запаковка выходного фрейма
            m_outframe.setSpecial({(char*)m_buffer.data(), nres, Raw});
        }

        //отправка по выходу
        Device::ioWrite(m_outframe);
    }
}

//----------------------------------------------------------------------------------------------
void TransformTB::setIntegerFormat(TransformTB::MidFormat format)
{
    m_format = format;
}

//----------------------------------------------------------------------------------------------
TransformTB::MidFormat TransformTB::getIntegerFormat() const
{
    return m_format;
}

//----------------------------------------------------------------------------------------------
int TransformTB::convert(const DspLab::real_t* x, int nx, unsigned char* data, int max_size,
                         TransformTB::MidFormat format)
{
    int nres = 0;
    for (int i = 0; i < nx; ++i) {
        switch (format) {
        case MID_INT8: {
            ((char*)data)[i] = round(x[i]);
            nres += 1;
            break;
        }

        case MID_INT16: {
            ((short*)data)[i] = round(x[i]);
            nres += 2;
            break;
        }

        case MID_INT24: {
            int value = round(x[i]);
            value = value << 8;
            data[i] = *((unsigned char*)(&value));
            data[i + 1] = *((unsigned char*)(&value) + 1);
            data[i + 2] = *((unsigned char*)(&value) + 2);
            nres += 3;
            break;
        }

        case MID_INT32: {
            ((int*)data)[i] = round(x[i]);
            nres += 4;
            break;
        }

        case MID_INT40: {
            long long int value = round(x[i]);
            value = value << 24;
            data[i] = *((unsigned char*)(&value));
            data[i + 1] = *((unsigned char*)(&value) + 1);
            data[i + 2] = *((unsigned char*)(&value) + 2);
            data[i + 3] = *((unsigned char*)(&value) + 3);
            data[i + 4] = *((unsigned char*)(&value) + 4);
            nres += 5;
            break;
        }

        default:
            break;
        }
    }

    assert(nres <= max_size);
    return nres;
}
