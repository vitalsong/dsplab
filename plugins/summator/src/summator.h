#pragma once

#include <dsplab/device.h>

#include <tfifobuffer.h>

namespace DspLab {

class Summator : public Device
{
public:
    Summator();
    virtual ~Summator();

    void processing() final;
    void start() final;
    void stop() final;

private:
    //фрейм для чтения
    Frame m_frame[2];

    //текущие форматы входных портов
    Format m_formats[2];

    //буферизация сигнала
    TFifoBuffer<real_t> m_in[2];

    //промежуточный буфер для хранения результата
    std::vector<real_t> m_out;

    //фрейм для записи
    Frame m_frameOut;

    //проверка активности входа
    bool isInputDisabled(int nin);

    //процессинг чтения и обработки для одного канала
    void channelProcess(int nin);

    //буферизация
    void buffering(int nin);

    //суммирование
    void summation();

    //вывод
    void writing();

    //хранение мета-информации потока
    DspMeta m_meta[2];
};

}   // namespace DspLab
