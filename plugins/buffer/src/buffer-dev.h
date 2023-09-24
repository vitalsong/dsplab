#pragma once

#include <dsplab/device.h>
#include <mutex>

namespace DspLab {

class BufferDev : public Device
{
public:
    BufferDev();
    virtual ~BufferDev();
    void processing() final;
    void setBufferSize(int size);
    int getBufferSize() const;

private:
    uint32_t m_buffer_size;
    std::vector<char> m_buffer_byte;
    std::vector<double> m_buffer_real;
    std::mutex m_mutex;
};

}   // namespace DspLab
