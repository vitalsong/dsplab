#pragma once

#include <dsplab/device.h>
#include <mutex>
#include <atomic>

class Timer;

namespace DspLab {

/*!
 * \brief Генератор синуса
 * \details Основные параметры могут быть изменены только,
 * когда устройство отключено.
 */
class SinusGenerator : public Device
{
public:
    SinusGenerator();
    virtual ~SinusGenerator();

    void start() final;
    void stop() final;
    void processing() final;

    /*!
     * \brief Задать частоту дискретизации
     * \param value Частота дискретизации в Гц
     */
    void setSampleRateHz(int value);

    /*!
     * \brief Получить текущую частоту дискретизации
     * \return Частота дискретизации в Гц
     */
    int getSampleRateHz() const;

    /*!
     * \brief Задать амплитуду тона в дБ
     * \param value Значение в дБ (относительно full scale)
     */
    void setAmplitudeDB(double value);

    /*!
     * \brief Получить текущую амплитуду тона в дБ
     * \return Значение в дБ (относительно full scale)
     */
    double getAmplitudeDB() const;

    /*!
     * \brief Задать "Full Scale"
     * \param value Значение в "попугаях"
     */
    void setFullScale(double value);

    /*!
     * \brief Получить текущий "Full Scale"
     * \return Значение в "попугаях"
     */
    double getFullScale() const;

    /*!
     * \brief Задать частоту тона
     * \param value Значение в Гц
     */
    void setFrequensyHz(int value);

    /*!
     * \brief Получить текущую частоту тона
     * \return Значение в Гц
     */
    int getFrequensyHz() const;

    /*!
     * \brief Задать период генерации фреймов с сэмплами
     * \param value Значение в мсек
     */
    void setGeneratePeriod(int value);

    /*!
     * \brief Получить период генерации фреймов с сэмплами
     * \return Значение в мсек
     */
    int getGeneratePeriod() const;

private:
    double m_ampPerDb;
    double m_fscale;
    int m_sampleRate;
    int m_period;
    int m_iphase;
    Frame m_frame;
    std::vector<real_t> m_buffer;

    //потокобезопасные переменные
    std::atomic_int m_frequency;
    std::atomic_int m_ampPerScale;
};

}   // namespace DspLab
