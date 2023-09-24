#pragma once

#include <dsplab/device.h>

namespace DspLab {

class PaprDev : public Device
{
public:
    PaprDev();
    virtual ~PaprDev();
    void processing() final;
    double value() const;
    void setMeasurePeriod(int nsmp);
    int measurePeriod();

private:
    void _update(const double* x, int nx);
    void _resetParams();

private:
    Frame m_frame;
    double m_rms;    ///< сумматор для расчета RMS
    double m_max;    ///< текущее значение максимума
    double m_papr;   ///< текущее значение PAPR
    int m_counter;   ///< счетчик отсчетов в сумматоре rms
    int m_period;
};
}   // namespace DspLab
