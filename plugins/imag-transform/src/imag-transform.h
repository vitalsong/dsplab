#pragma once

#include <dsplab/device.h>

namespace DspLab {

class ImagTransform : public Device
{
public:
    ImagTransform();
    virtual ~ImagTransform();
    void processing() final;

private:
    //фреймы для буферизации
    Frame m_frameIn;
    Frame m_frameOut;

    //промежуточный буфер для преобразований
    std::vector<real_t> m_buffer;
};

}   // namespace DspLab
