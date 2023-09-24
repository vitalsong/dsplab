#pragma once

namespace DspLab {

/*!
 * \brief Структура для хранения универсальных метаданных фрейма
 */
struct DspMeta
{
    DspMeta(double _sampleRate = 1, double _centerFreq = 0.5, double _scalePerParrots = 32767,
            double _scalePerVolts = 1)
    {
        sampleRate = _sampleRate;
        centerFreq = _centerFreq;
        scalePerParrots = _scalePerParrots;
        scalePerVolts = _scalePerVolts;
    }

    double sampleRate;   ///< частота дискретизации (или ширина полосы)
    double centerFreq;   ///< центральная частота (если сигнал после приемника)
    double scalePerParrots;   ///< размах шкалы в "попугаях"
    double scalePerVolts;     ///< размах шкалы в вольтах
};

}   // namespace DspLab
