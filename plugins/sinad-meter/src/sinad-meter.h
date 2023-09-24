#pragma once

#include <QObject>
#include <mutex>

class SinadMeter : public QObject
{
    Q_OBJECT

public:
    explicit SinadMeter(QObject* parent = nullptr);

    //настройка основания БПФ для расчетов
    void setFftSize(int nfft);
    int fftSize() const;

    //задать степень усреднения БПФ выборок
    void setFftAccumulation(int count);
    int fftAccumulation() const;

    //включить/выключить режим расчета с фиксированной позицией
    void setFixedMode(bool enabled);
    bool isFixedMode() const;

    //задать фиксированный бин для расчета
    void setFixedPeakBin(int ibin);
    int fixedPeakBin() const;

    //текущее значение sinad
    double value() const;

    //запись в модуль расчета
    //ЗЫ: все расчеты происходят в этой ф-ии
    void process(const double* x, int nx);

    //сброс внутреннего состояния
    void reset();

signals:
    void valueChanged(double dB);

private:
    //расчет спектра
    void _calculateSpectrum();

    //оценка синад по спектру
    void _calculateSinad();

    mutable std::mutex m_mutex;
    double m_sinad;                   ///< текущее значение
    int m_nfft;                       ///< размер основания БПФ под расчеты
    std::vector<double> m_buffer;     ///< буфер под накопление сигнала
    std::vector<double> m_spectrum;   ///< буфер под накопление спектра
    int m_buffer_load;                ///< степень наполненности буфера под сигнал
    bool m_fixed_freq;                ///< активность режима с фиксированным бином
    int m_freq_num;                   ///< номер фиксированного бина
    int m_accum_period;               ///< заданная степень накопления спектра
    int m_accum_count;                ///< текущая степень накопленности спектра
};
