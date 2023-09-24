#pragma once

#include <dsplab/device.h>

namespace DspLab {

class RealTransform : public Device
{
public:
    RealTransform();
    virtual ~RealTransform();
    void processing() final;

private:
    //фреймы для буферизации
    Frame m_frameIn;
    Frame m_frameOut;

    //промежуточный буфер для преобразований
    std::vector<real_t> m_buffer;
};

}   // namespace DspLab
