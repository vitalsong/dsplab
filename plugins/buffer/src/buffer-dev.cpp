#include "buffer-dev.h"
#include <string>
#include <stdlib.h>
#include <cstdlib>

using namespace DspLab;

//-------------------------------------------------------------------------------------------------
BufferDev::BufferDev()
  : Device(1, 1)
  , m_buffer_size(512)
{
    Device::setAvaliableInputFormats({Real, Complex, Special});
    Device::setAvaliableOutputFormats({Real, Complex, Special});
}

//-------------------------------------------------------------------------------------------------
BufferDev::~BufferDev()
{
    BufferDev::stop();
}

//-------------------------------------------------------------------------------------------------
void BufferDev::processing()
{
    Frame frame;
    while (Device::ioSize() > 0) {
        std::lock_guard<std::mutex> locker(m_mutex);

        //чтение из порта
        Device::ioRead(frame);

        //распаковка фрейма
        if (frame.format() == Special) {
            auto package = frame.getSpecial();
            const char* data = package.data();
            int size = package.size();

            for (int i = 0; i < size; ++i) {
                m_buffer_byte.push_back(data[i]);
                if (m_buffer_byte.size() >= m_buffer_size) {
                    frame.setSpecial({m_buffer_byte.data(), (int)m_buffer_byte.size(), Raw});
                    m_buffer_byte.clear();
                    Device::ioWrite(frame);
                }
            }
        }

        if (frame.format() == Real) {
            auto package = frame.getReal();
            const real_t* data = package.data();
            int size = package.size();
            for (int i = 0; i < size; ++i) {
                m_buffer_real.push_back(data[i]);
                if (m_buffer_real.size() >= m_buffer_size) {
                    frame.setReal({(real_t*)m_buffer_real.data(), (int)m_buffer_real.size()});
                    m_buffer_real.clear();
                    Device::ioWrite(frame);
                }
            }
        }

        if (frame.format() == Complex) {
            auto package = frame.getComplex();
            const complex_t* data = package.data();
            int size = package.size();
            for (int i = 0; i < size; ++i) {
                m_buffer_real.push_back(data[i].xi);
                m_buffer_real.push_back(data[i].xq);
                if (m_buffer_real.size() >= 2 * m_buffer_size) {
                    frame.setComplex({(complex_t*)m_buffer_real.data(), (int)m_buffer_real.size() / 2});
                    m_buffer_real.clear();
                    Device::ioWrite(frame);
                }
            }
        }
    }
}

//-------------------------------------------------------------------------------------------------
void BufferDev::setBufferSize(int size)
{
    std::lock_guard<std::mutex> locker(m_mutex);
    m_buffer_size = size;
}

//-------------------------------------------------------------------------------------------------
int BufferDev::getBufferSize() const
{
    return m_buffer_size;
}
