#pragma once

#include <dsplab/device.h>
#include <QHostAddress>

#include <thread-socket.h>

namespace DspLab {

class UdpSenderDev : public Device
{
public:
    UdpSenderDev();
    virtual ~UdpSenderDev();
    void processing() final;

    /*!
     * \brief Хранение параметров адресатов
     */
    struct Transmit
    {
        Transmit(QHostAddress _addr = QHostAddress::LocalHost, int _port = 0)
          : addr(_addr)
          , port(_port)
        {}
        QHostAddress addr;
        int port;
    };

    /*!
     * \brief Добавить получателя
     * \param tr Информация о получателе
     * \return Успех операции
     */
    bool addTransmit(const Transmit& tr);

    /*!
     * \brief Удалить получателя
     * \param tr Информация о получателе
     * \return Успех операции
     */
    bool delTransmit(const Transmit& tr);

    /*!
     * \brief Получить список получателей
     * \return Список получателей
     */
    std::list<Transmit> transmitList() const;

private:
    Frame m_frame;
    ThreadSocket* m_socket;
};

}   // namespace DspLab
