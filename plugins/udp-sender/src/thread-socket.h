#pragma once

#include <QThread>
#include <QUdpSocket>
#include <QByteArray>
#include <QQueue>

#include <mutex>
#include <atomic>
#include <condition_variable>

/*!
 * \brief Класс для отправки UDP дейтаграмм в отдельном потоке
 * \details Не "фризится" от Qt графики и прочих задач
 */
class ThreadSocket : public QThread
{
public:
    using Transmit = QPair<QHostAddress, quint16>;

    ThreadSocket();
    virtual ~ThreadSocket();
    void run() final;
    void addTransmit(const QHostAddress& address, quint16 port);
    void delTransmit(const QHostAddress& address, quint16 port);
    void send(const char* data, quint64 size);
    QList<Transmit> transmitList() const;

private:
    std::mutex m_paramLock;
    std::mutex m_sendLock;
    std::mutex m_deleteLock;
    QQueue<QByteArray> m_querry;
    QList<Transmit> m_transmits;
    std::condition_variable m_checker;
    bool m_dataReady;
    bool m_deleteReady;
};
