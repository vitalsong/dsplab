#pragma once

#include <dsplab/device.h>

namespace DspLab {

class AwgnDev : public Device
{
public:
    AwgnDev();
    virtual ~AwgnDev();
    void processing() final;

    void setSnr(double snr);
    double getSnr();

    void setExpCoeff(double alpha);
    double getExpCoeff();

private:
    void _processReal(const Frame& frame);
    void _processComplex(const Frame& frame);

private:
    Frame m_inframe;
    Frame m_outframe;
    double m_rms;
    double m_rmsxq;
    double m_snr;
    double m_expect;
    double m_smth;
    std::vector<double> m_real;
    std::vector<complex_t> m_complex;
}

;
}   // namespace DspLab
