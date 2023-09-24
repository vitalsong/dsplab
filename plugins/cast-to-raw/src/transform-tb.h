#pragma once

#include <dsplab/device.h>

namespace DspLab {

class TransformTB : public Device
{
public:
    TransformTB();
    virtual ~TransformTB();

    enum MidFormat
    {
        MID_INT8,
        MID_INT16,
        MID_INT24,
        MID_INT32,
        MID_INT40
    };

    //основная функция обработки
    void processing() final;

    //установить промежуточный формат для преобразования
    void setIntegerFormat(MidFormat format);

    //получить промежуточный формат для преобразования
    MidFormat getIntegerFormat() const;

private:
    //преобразование типа
    static int convert(const real_t* x, int nx, unsigned char* data, int max_size, MidFormat format);

    //фреймы для приема
    Frame m_inframe;
    Frame m_outframe;

    //промежуточный формат
    MidFormat m_format;

    //промежуточный буфер для преобразований
    std::vector<unsigned char> m_buffer;
};

}   // namespace DspLab
