#pragma once

#include <dsplab/device.h>

#include <vector>

namespace DspLab {

/*!
 * \brief Генератор белого шума
 * \details Используется нормальное Гаусcово распределение
 * \todo Необходимо перейти от ст.отклонения к мощности шума в дБ и убрать настройку мат.ожидания
 */
class NoiseGenerator : public Device
{
public:
    NoiseGenerator();
    virtual ~NoiseGenerator();
    void processing() final;

    //комплексный формат
    void setComplexTypeEnabled(bool enabled);
    bool isComplexTypeEnabled() const;

    /*!
     * \brief Задать период генерации
     * \param msec Период генерации в мсек
     */
    void setGeneratePeriod(int msec);

    /*!
     * \brief Текущий период генерации
     * \return Период генерации в мсек
     */
    int generatePeriod() const;

    /*!
     * \brief Задать частоту дискретизации сигнала
     * \param value Частота дискретизации (Гц)
     */
    void setSampleRate(int value);

    /*!
     * \brief Текущая частота дискретизации сигнала
     * \return Частота дискретизации (Гц)
     */
    int sampleRate() const;

    /*!
     * \brief Задать мат.ожидание Гауссова шума
     * \param value Мат ожидание
     */
    void setExpectation(double value);

    /*!
     * \brief Текущее мат.ожидание Гауссова шума
     * \return Мат.ожидание
     */
    double expectation() const;

    /*!
     * \brief Задать стандартное отклонение Гауссова шума
     * \param value Стандартное отклонение
     */
    void setDeviation(double value);

    /*!
     * \brief Текущее стандартное отклонение Гауссова шума
     * \return Стандартное отклонение
     */
    double deviation() const;

private:
    bool m_bcomplex;

    //функция генерации шума
    static void generateNoise(real_t* x, int nx, double m, double q);

    //период таймера
    int m_period;

    //частота дискретизации
    int m_sampleRate;

    //период генерации в сэмплах
    int m_sampleCount;

    //параметры шума
    double m_expectation;
    double m_deviation;

    //фрейм для буферизации
    Frame m_frame;

    //буферизация сигнала
    std::vector<real_t> m_buffer;

    //защита при вызове функций
    std::mutex m_mutex;
};

}   // namespace DspLab
