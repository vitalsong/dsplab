#pragma once

#include <dsplab/device.h>

class SinadMeter;

namespace DspLab {
class SinadDev : public Device
{
    Q_OBJECT

public:
    SinadDev();
    virtual ~SinadDev();
    void processing() final;
    double value() const;

private:
    Frame m_frame;
    SinadMeter* m_meter;
    double m_value;

private slots:
    void onSinadValueChanged(double value);
};

}   // namespace DspLab
