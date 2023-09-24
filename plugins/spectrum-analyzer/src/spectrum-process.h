#ifndef SPECTRUM_PROCESS_H
#define SPECTRUM_PROCESS_H

#include <QObject>
#include <QVector>
#include <QPointF>

#include <dsplib.h>

#include "spectrum-common.h"

namespace Spectrum {

/*!
 * \brief Класс для расчета набора точек спектра сигнала
 * \details Формирует набор точек вида V[N][2], где N - размерность БПФ
 */
class Process : public QObject
{
    Q_OBJECT

public:
    Process();

public slots:

    /*!
     * \brief Задать основание БПФ
     * \details Если не является степенью 2-ки, то округляется до ближайшего
     * \warning Значения меньше 32 ничего не изменяют
     * \param nfft Основание БПФ
     */
    void setFftSize(int nfft);

    /*!
     * \brief Текущее основание БПФ
     * \return Основание БПФ (всегда степень 2-ки)
     */
    int getFftSize() const;

    /*!
     * \brief Размах сетки
     * \param scale Значение размаха (например 2^15-1 для 16-бит)
     */
    void setFullScale(double scale);

    /*!
     * \brief Текущий размах сетки
     * \return Значение размаха
     */
    double getFullScale() const;

    /*!
     * \brief Настройка оконной функции
     * \param wind Тип оконной функции
     */
    void setFftWindow(FftWindow wind);

    /*!
     * \brief Текущая оконная функция
     * \return Тип оконной функции
     */
    FftWindow getFftWindow() const;

    /*!
     * \brief Кол-во БПФ для накопления
     * \details Используется только для режима AccumulationMean
     * \param count Кол-во выборок БПФ, которые будут усреднены
     */
    void setAccumulationRate(int count);

    /*!
     * \brief Текущее кол-во БПФ выборок для накопления
     * \return Кол-во выборок БПФ
     */
    int getAccumulationRate() const;

    /*!
     * \brief Режим накопления спектра
     * \param mode Режим накопления
     */
    void setAccumulationMode(AccumulationMode mode);

    /*!
     * \brief Текущий режим накопления спектра
     * \return Режим накопления
     */
    AccumulationMode getAccumulationMode() const;

    /*!
     * \brief Частота дискретизации входного сигнала
     * \details Используется для формирования оси Х
     * \param sampleRate Частота дискретизации (Гц)
     */
    void setSampleRate(qint64 sampleRate);

    /*!
     * \brief Текущая частота дискретизации
     * \return Частота дискретизации (Гц)
     */
    quint64 getSampleRate() const;

    /*!
     * \brief Задать кол-во пиков для расчета
     * \param count Кол-во пиков
     */
    void setPeaksCount(int count);

    /*!
     * \brief Текущее кол-во пиков
     * \return Кол-во пиков
     */
    int getPeaksCount() const;

    /*!
     * \brief Запись сигнала
     * \details При достаточном кол-ве данных будет произведен расчет
     * и сгенерирован сигнал spectrumReady с результатом.
     * При смене типа данных происходит автоматический сброс накопления.
     * \param sig Фрагмент реального сигнала
     */
    void write(const dsplib::arr_real& sig);

    /*!
     * \brief Запись сигнала
     * \param sig Фрагмент комплексного сигнала
     */
    void write(const dsplib::arr_cmplx& sig);

    /*!
     * \brief Cброс текущего состояния вычислений
     */
    void reset();

signals:

    /*!
     * \brief Сигнал, генерируемый при готовности точек спектра
     * \param spectrum Вектор с набором точек для отрисовки спектра (частота, амплитуда)
     */
    void spectrumReady(QVector <QPointF> spectrum);

    /*!
     * \brief Сигнал о завершении расчета пиковых гармоник
     * \param peaks Вектор первых N пиковых гармоник (частота, амплитуда)
     */
    void peaksReady(QVector <QPointF> peaks);

private:
    void _writeToBuffer(const dsplib::arr_cmplx& sig);    //запись в буферы
    void _refreshBuffers();     //обновление размеров буфера
    void _refreshWindow();      //перерасчет оконной функции
    void _calculate();          //расчет БПФ

    //накопление спектра
    void _updateFftAbs(const dsplib::arr_real& fft_abs);
    void _accumOnce(const dsplib::arr_real& fft_abs);
    void _accumMean(const dsplib::arr_real& fft_abs);
    void _accumMax(const dsplib::arr_real& fft_abs);
    void _accumMin(const dsplib::arr_real& fft_abs);

    //формирование спектра
    void _formSpectrumPoints(const dsplib::arr_real& spectrum);

    //формирование списка пиковых частот
    void _formPeaksPoints(const dsplib::arr_real& spectrum);

private:
    dsplib::arr_cmplx m_fft_buf;    ///< буферизация комплексного сигнала
    int m_fft_load;                 ///< заполненность буфера
    dsplib::arr_real m_wind;        ///< буферизация оконной функции
    int m_nfft;                     ///< основание БПФ
    double m_scale;                 ///< размах шкалы
    Spectrum::FftWindow m_wind_mode;    ///< тип оконной функции
    double m_cgain;                 ///< корреция для текущего окна
    int m_accum_rate;               ///< степень накопления
    int m_accum_count;              ///< счетчик усредненных БПФ фреймов
    Spectrum::AccumulationMode m_accum_mode;  ///< режим накопления
    qint64 m_sample_rate;           ///< частота дискретизации
    dsplib::arr_real m_accum_amp;   ///< буфер с накопленными результатами вычислений
    bool m_breal;                   ///< флаг реального сигнала на входе
    int m_peaks_count;
};

} ///< Spectrum

#endif // SPECTRUM_PROCESS_H
