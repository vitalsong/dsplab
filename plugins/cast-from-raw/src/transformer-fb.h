#pragma once

#include <dsplab/device.h>

namespace DspLab {

class TransformerFB : public Device
{
public:
    TransformerFB();
    virtual ~TransformerFB();
    void processing() final;

    /*!
     * \brief Промежуточный формат для преобразования
     */
    enum MidFormat
    {
        InInt8,
        InInt16,
        InInt24,
        InInt32,
        InInt40
    };

    /*!
     * \brief Выходной формат
     */
    enum OutFormat
    {
        OutReal,
        OutComplex
    };

    //настройка промежуточного формата
    void setMidFormat(MidFormat format);
    MidFormat getMidFormat() const;

    //задать выходной формат
    void setOutFormat(OutFormat format);
    OutFormat getOutFormat() const;

private:
    //преобразование данных из буфера
    void convertDataToInt();

    //фреймы для приема
    Frame m_inFrame;
    Frame m_outFrame;

    //промежуточный формат
    MidFormat m_midFormat;

    //промежуточный буфер для преобразований
    std::vector<char> m_midBuffer;

    //выходной формат
    OutFormat m_outFormat;

    //промежуточный буфер для хранения распакованных значений
    std::vector<real_t> m_outBuffer;
};

}   // namespace DspLab
