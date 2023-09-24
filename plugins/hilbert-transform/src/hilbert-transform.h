#pragma once

#include <dsplab/device.h>
#include <dsplib/hilbert.h>
#include <memory>

namespace DspLab {

class HilbertTransform : public Device
{
public:
    HilbertTransform();
    virtual ~HilbertTransform();
    void processing() final;

private:
    typedef std::shared_ptr<dsplib::HilbertFilter> HilbertPtr;
    HilbertPtr m_hilbert;
    Frame m_frameIn;
    Frame m_frameOut;
};

}   // namespace DspLab
