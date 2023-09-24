#include <udp-sender-dev.h>

using namespace DspLab;

//-------------------------------------------------------------------------------------------------
UdpSenderDev::UdpSenderDev()
  : Device(1, 0)
{
    Device::setAvaliableInputFormats({Special});
    m_socket = new ThreadSocket();
    m_socket->start();
}

//-------------------------------------------------------------------------------------------------
UdpSenderDev::~UdpSenderDev()
{
    UdpSenderDev::stop();
    delete m_socket;
}

//-------------------------------------------------------------------------------------------------
void UdpSenderDev::processing()
{
    while (Device::ioSize() > 0) {
        //чтение из входного порта
        Device::ioRead(m_frame);

        auto package = m_frame.getSpecial();
        const char* data = package.data();
        int size = package.size();

        //отправка
        m_socket->send(data, size);
    }
}

//-------------------------------------------------------------------------------------------------
bool UdpSenderDev::addTransmit(const UdpSenderDev::Transmit& tr)
{
    try {
        m_socket->addTransmit(tr.addr, tr.port);
    } catch (...) {
        return false;
    }

    return true;
}

//-------------------------------------------------------------------------------------------------
bool UdpSenderDev::delTransmit(const UdpSenderDev::Transmit& tr)
{
    try {
        m_socket->delTransmit(tr.addr, tr.port);
    } catch (...) {
        return false;
    }

    return true;
}

//-------------------------------------------------------------------------------------------------
std::list<UdpSenderDev::Transmit> UdpSenderDev::transmitList() const
{
    std::list<UdpSenderDev::Transmit> list;
    auto ls = m_socket->transmitList();

    for (auto it : ls) {
        list.push_back({it.first, it.second});
    }

    return list;
}
