#include "thread-socket.h"
#include <QUdpSocket>

//-------------------------------------------------------------------------------------------------
ThreadSocket::ThreadSocket()
{
    m_dataReady = false;
    m_deleteReady = false;
}

//-------------------------------------------------------------------------------------------------
ThreadSocket::~ThreadSocket()
{
    m_deleteReady = true;
    m_checker.notify_one();
    std::lock_guard<std::mutex> locker(m_deleteLock);
}

//-------------------------------------------------------------------------------------------------
void ThreadSocket::run()
{
    //блокируем удаление объекта до завершения выполнения
    std::lock_guard<std::mutex> locker(m_deleteLock);

    //создаем сокет и привязываем к потоку
    QUdpSocket* socket = new QUdpSocket();
    socket->moveToThread(this);

    //пока не пришел запрос на удаление
    while (!m_deleteReady) {
        //ожидание нового пакета данных в очереди (либо запрос на удаление объекта)
        std::unique_lock<std::mutex> sendLocker(m_sendLock);
        m_checker.wait(sendLocker, [&]() {
            return (m_dataReady || m_deleteReady);
        });

        //блокируем возможность изменения параметров
        std::unique_lock<std::mutex> paramLocker(m_paramLock);

        //если в очереди есть данные
        if (!m_querry.isEmpty()) {
            QByteArray pack = m_querry.dequeue();
            for (const Transmit& tr : m_transmits) {
                socket->writeDatagram(pack, tr.first, tr.second);
            }
        } else {
            //если очередь пуста
            m_dataReady = false;
        }
    }

    delete socket;
}

//-------------------------------------------------------------------------------------------------
void ThreadSocket::addTransmit(const QHostAddress& address, quint16 port)
{
    std::lock_guard<std::mutex> locker(m_paramLock);

    for (const Transmit& it : m_transmits) {
        if ((it.first == address) && (it.second == port)) {
            throw(std::runtime_error("Transmit already exist."));
            return;
        }
    }

    m_transmits.push_back({address, port});
}

//-------------------------------------------------------------------------------------------------
void ThreadSocket::delTransmit(const QHostAddress& address, quint16 port)
{
    std::lock_guard<std::mutex> locker(m_paramLock);

    auto it = m_transmits.begin();
    while (it != m_transmits.end()) {
        if ((it->first == address) && (it->second == port)) {
            m_transmits.erase(it);
            return;
        }

        ++it;
    }

    throw(std::runtime_error("Transmit does not exist."));
}

//-------------------------------------------------------------------------------------------------
void ThreadSocket::send(const char* data, quint64 size)
{
    std::lock_guard<std::mutex> locker(m_paramLock);
    m_querry.enqueue(QByteArray(data, size));
    m_dataReady = true;
    m_checker.notify_one();
}

//-------------------------------------------------------------------------------------------------
QList<ThreadSocket::Transmit> ThreadSocket::transmitList() const
{
    return m_transmits;
}
