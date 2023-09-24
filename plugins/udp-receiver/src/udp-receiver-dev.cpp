#include "udp-receiver-dev.h"
#include <algorithm>

using namespace DspLab;

//-------------------------------------------------------------------------------------------------
UdpReceiverDev::UdpReceiverDev()
  : Device(0, 1)
{
    //настройка выходного формата (байтовый спец.формат)
    Device::setAvaliableOutputFormats({Special});

    //настройка сокета
    m_socket = new QUdpSocket();
    connect(m_socket, SIGNAL(readyRead()), this, SLOT(onReadDatagramm()));
}

//-------------------------------------------------------------------------------------------------
UdpReceiverDev::~UdpReceiverDev()
{
    UdpReceiverDev::stop();
    delete m_socket;
}

//-------------------------------------------------------------------------------------------------
void UdpReceiverDev::start()
{
    if (m_socket->bind(m_port)) {
        Device::start();
        return;
    } else {
        Device::stop();
        return;
    }
}

//-------------------------------------------------------------------------------------------------
void UdpReceiverDev::stop()
{
    m_socket->close();
    Device::stop();
}

//-------------------------------------------------------------------------------------------------
void UdpReceiverDev::processing()
{
    //nothing to do
}

//-------------------------------------------------------------------------------------------------
void UdpReceiverDev::setBindPort(quint16 port)
{
    m_port = port;
}

//-------------------------------------------------------------------------------------------------
quint16 UdpReceiverDev::bindPort() const
{
    return m_port;
}

//-------------------------------------------------------------------------------------------------
void UdpReceiverDev::setAddressFilter(UdpReceiverDev::Address addr)
{
    m_filter = addr;
}

//-------------------------------------------------------------------------------------------------
UdpReceiverDev::Address UdpReceiverDev::addressFilter() const
{
    return m_filter;
}

//-------------------------------------------------------------------------------------------------
std::vector<UdpReceiverDev::Address> UdpReceiverDev::connectionList() const
{
    return m_connections;
}

//-------------------------------------------------------------------------------------------------
void UdpReceiverDev::refresh()
{
    m_connections.clear();
    emit sgConnectionListUpdated();
}

//-------------------------------------------------------------------------------------------------
void UdpReceiverDev::onReadDatagramm()
{
    QHostAddress ip;
    quint16 port;

    //если в очереди есть датаграммы
    while (m_socket->hasPendingDatagrams()) {
        //чтение датаграммы
        m_datagramm.resize(m_socket->pendingDatagramSize());
        int size = m_socket->readDatagram(m_datagramm.data(), m_datagramm.size(), &ip, &port);

        //если адрес для отправки не был назначен
        if (m_connections.size() == 0) {
            m_filter = {ip, port};
            m_connections.push_back(m_filter);
            emit sgConnectionListUpdated();
        }

        //если адрес совпадает c текущим
        if ((m_filter.first == ip) && (m_filter.second == port)) {
            //заносим во фрейм
            m_frame.setSpecial({m_datagramm.data(), size, Raw});

            //пишем по выходу
            Device::ioWrite(m_frame);
        } else {
            //если подключение существует
            for (const Address& it : m_connections) {
                if ((it.first == ip) && (it.second == port)) {
                    return;
                }
            }

            //добавляем в список
            m_connections.push_back({ip, port});

            emit sgConnectionListUpdated();
        }
    }
}
