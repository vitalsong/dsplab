#pragma once

#include <dsplab/device.h>

#include <memory>

namespace DspLab {

/*!
 * \brief Модуль управления уровнем
 */
class Gain : public Device
{
public:
    Gain();
    virtual ~Gain();

    void processing() final;

    /*!
     * \brief Изменить уровень усиления
     * \param value Уровень усиления в дБ
     */
    void changeGain(double value);

    /*!
     * \brief Текущий уровень усиления
     * \return Уровень усиления в дБ
     */
    double currentGain() const;

private:
    double m_gain;
    double m_mlt;
    Frame m_frameIn;
    Frame m_frameOut;
    std::vector<real_t> m_buffer;
};

}   // namespace DspLab
