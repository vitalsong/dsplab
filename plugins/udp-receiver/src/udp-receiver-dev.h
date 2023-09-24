#pragma once

#include <dsplab/device.h>

#include <QObject>
#include <QUdpSocket>
#include <QVector>

namespace DspLab {

class UdpReceiverDev : public Device
{
    Q_OBJECT

public:
    UdpReceiverDev();
    virtual ~UdpReceiverDev();

    void start() final;
    void stop() final;
    void processing() final;

    typedef std::pair<QHostAddress, quint16> Address;

    //текущий порт сокета
    void setBindPort(quint16 port);
    quint16 bindPort() const;

    //установить фильтр адреса для приема
    void setAddressFilter(Address addr);
    Address addressFilter() const;

    //получить список подключений
    std::vector<Address> connectionList() const;

    //обновить список подключений
    void refresh();

private:
    std::vector<Address> m_connections;   ///< список входных подключений
    QByteArray m_datagramm;               ///< буфер для приема датаграммы
    QUdpSocket* m_socket;                 ///< сокет приема
    Address m_filter;                     ///< параметры фильтра приема
    quint16 m_port;                       ///< текущий порт приема
    Frame m_frame;                        ///< фрейм для буферизации

private slots:
    void onReadDatagramm();

signals:
    void sgConnectionListUpdated();
};

}   // namespace DspLab
