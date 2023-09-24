#include "imag-transform.h"

#include <string>
#include <stdlib.h>

using namespace DspLab;

//-------------------------------------------------------------------------------------------------
ImagTransform::ImagTransform()
  : Device(1, 1)
{
    //настройка портов по-умолчанию
    Device::setAvaliableInputFormats({Complex});
    Device::setAvaliableOutputFormats({Real});
}

//-------------------------------------------------------------------------------------------------
ImagTransform::~ImagTransform()
{
    ImagTransform::stop();
}

//-------------------------------------------------------------------------------------------------
void ImagTransform::processing()
{
    while (Device::ioSize() > 0) {
        //чтение из порта
        Device::ioRead(m_frameIn);

        //определяем содержимое фрейма
        auto package = m_frameIn.getComplex();
        const complex_t* inCmplx = package.data();
        int size = package.size();
        DspMeta meta = package.meta();

        //преобразование типа
        m_buffer.resize(size);
        real_t* outReal = m_buffer.data();
        for (int i = 0; i < size; ++i) {
            outReal[i] = inCmplx[i].xq;
        }

        //запаковка фрейма
        m_frameOut.setReal({outReal, size, meta});

        //отправка на выход
        Device::ioWrite(m_frameOut);
    }
}
