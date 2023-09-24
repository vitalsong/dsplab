#include "frame.h"

#include <string.h>

using namespace DspLab;

//---------------------------------------------------------------------------------
struct Frame::Impl
{
    Format format;
    std::vector<char> raw_buf;
    std::vector<real_t> real_buf;
    DspMeta meta;
};

//---------------------------------------------------------------------------------
Frame::Frame()
{
    m_impl = std::shared_ptr<Impl>(new Impl());
    m_impl->format = Format(Special, Raw);
}

//---------------------------------------------------------------------------------
Frame::Frame(const Frame& frame)
{
    m_impl = std::shared_ptr<Impl>(new Impl());
    *(m_impl.get()) = *(frame.m_impl.get());
}

//---------------------------------------------------------------------------------
Frame::Frame(Frame::RealData data)
{
    m_impl = std::shared_ptr<Impl>(new Impl());
    setReal(data);
}

//---------------------------------------------------------------------------------
Frame::Frame(Frame::ComplexData data)
{
    m_impl = std::shared_ptr<Impl>(new Impl());
    setComplex(data);
}

//---------------------------------------------------------------------------------
Frame::Frame(Frame::SpecialData data)
{
    m_impl = std::shared_ptr<Impl>(new Impl());
    setSpecial(data);
}

//---------------------------------------------------------------------------------
Format Frame::format() const
{
    return m_impl->format;
}

//---------------------------------------------------------------------------------
Frame::RealData Frame::getReal() const
{
    if (m_impl->format == Special) {
        return RealData(0, 0);
    }

    const real_t* data = m_impl->real_buf.data();
    int size = m_impl->real_buf.size();

    return RealData(data, size, m_impl->meta);
}

//---------------------------------------------------------------------------------
Frame::ComplexData Frame::getComplex() const
{
    if (m_impl->format == Special) {
        return ComplexData(0, 0);
    }

    const complex_t* data = (const complex_t*)m_impl->real_buf.data();
    int size = m_impl->real_buf.size() / 2;

    return ComplexData(data, size, m_impl->meta);
}

//---------------------------------------------------------------------------------
Frame::SpecialData Frame::getSpecial() const
{
    if (m_impl->format != Special) {
        return SpecialData(0, 0, 0);
    }

    const char* data = m_impl->raw_buf.data();
    int size = m_impl->raw_buf.size();
    int id = m_impl->format.special();

    return SpecialData(data, size, id);
}

//---------------------------------------------------------------------------------
void Frame::setReal(Frame::RealData pack)
{
    m_impl->format = Real;
    m_impl->meta = pack.meta();
    m_impl->real_buf.resize(pack.size());
    memcpy(m_impl->real_buf.data(), pack.data(), pack.size() * sizeof(real_t));
}

//---------------------------------------------------------------------------------
void Frame::setComplex(Frame::ComplexData pack)
{
    m_impl->format = Complex;
    m_impl->meta = pack.meta();
    m_impl->real_buf.resize(pack.size() * 2);
    memcpy(m_impl->real_buf.data(), (real_t*)pack.data(), pack.size() * sizeof(complex_t));
}

//---------------------------------------------------------------------------------
void Frame::setSpecial(Frame::SpecialData pack)
{
    m_impl->format = Format(Special, pack.id());
    m_impl->raw_buf.resize(pack.size());
    memcpy(m_impl->raw_buf.data(), pack.data(), pack.size() * sizeof(char));
}

//---------------------------------------------------------------------------------
Frame::RealData::RealData(const real_t* data, int size)
{
    m_data = data;
    m_size = size;
}

//---------------------------------------------------------------------------------
Frame::RealData::RealData(const real_t* data, int size, DspMeta meta)
{
    m_data = data;
    m_size = size;
    m_meta = meta;
}

//---------------------------------------------------------------------------------
const real_t* Frame::RealData::data() const
{
    return m_data;
}

//---------------------------------------------------------------------------------
int Frame::RealData::size() const
{
    return m_size;
}

//---------------------------------------------------------------------------------
DspMeta Frame::RealData::meta() const
{
    return m_meta;
}

//---------------------------------------------------------------------------------
Frame::ComplexData::ComplexData(const complex_t* data, int size)
{
    m_data = data;
    m_size = size;
}

//---------------------------------------------------------------------------------
Frame::ComplexData::ComplexData(const complex_t* data, int size, DspMeta meta)
{
    m_data = data;
    m_size = size;
    m_meta = meta;
}

//---------------------------------------------------------------------------------
const complex_t* Frame::ComplexData::data() const
{
    return m_data;
}

//---------------------------------------------------------------------------------
int Frame::ComplexData::size() const
{
    return m_size;
}

//---------------------------------------------------------------------------------
DspMeta Frame::ComplexData::meta() const
{
    return m_meta;
}

//---------------------------------------------------------------------------------
Frame::SpecialData::SpecialData(const char* data, int size, int id)
{
    m_data = data;
    m_size = size;
    m_id = id;
}

//---------------------------------------------------------------------------------
const char* Frame::SpecialData::data() const
{
    return m_data;
}

//---------------------------------------------------------------------------------
int Frame::SpecialData::size() const
{
    return m_size;
}

//---------------------------------------------------------------------------------
int Frame::SpecialData::id() const
{
    return m_id;
}
