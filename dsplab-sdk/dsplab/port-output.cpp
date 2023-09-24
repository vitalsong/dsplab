#include "port-output.h"
#include "port-input.h"

#include <algorithm>

using namespace DspLab;

//--------------------------------------------------------
PortOutput::PortOutput(IDevice* parent)
{
    m_parent = parent;
}

//--------------------------------------------------------
PortOutput::~PortOutput()
{
    std::lock_guard<std::mutex> locker(m_mutex);
    _detachAll();
}

//--------------------------------------------------------
IDevice* PortOutput::parent() const
{
    return m_parent;
}

//--------------------------------------------------------
std::vector<Format> PortOutput::avaliableFormats() const
{
    std::lock_guard<std::mutex> locker(m_mutex);
    return m_formats;
}

//--------------------------------------------------------
void PortOutput::setAvaliableFormats(std::vector<Format> format)
{
    std::lock_guard<std::mutex> locker(m_mutex);
    m_formats = format;
}

//--------------------------------------------------------
bool PortOutput::attach(IPortInput* _port)
{
    std::lock_guard<std::mutex> locker(m_mutex);
    PortInput* port = dynamic_cast<PortInput*>(_port);

    //если есть обратная связь
    if (m_parent == port->m_parent) {
        return false;
    }

    //если входной порт уже имеет коммутацию (но с другим выходным портом!)
    const auto& list = port->m_connections;
    if ((list.size() == 1) && (list.at(0) != this)) {
        return false;
    }

    //если форматы не совместимы
    if (!_portCompatibility(port)) {
        return false;
    }

    //если этот порт уже есть в списке
    auto it = std::find(m_connections.begin(), m_connections.end(), port);
    if (it != m_connections.end()) {
        return false;
    } else {
        m_connections.push_back(port);
    }

    //взаимное подключение
    auto f = std::find(list.begin(), list.end(), this);
    if (f == list.end()) {
        port->m_connections.push_back(this);
        return true;
    }

    return false;
}

//--------------------------------------------------------
bool PortOutput::detach(IPortInput* _port)
{
    std::lock_guard<std::mutex> locker(m_mutex);
    PortInput* port = dynamic_cast<PortInput*>(_port);

    auto it = std::find(m_connections.begin(), m_connections.end(), port);
    if (it == m_connections.end()) {
        return false;
    }

    //очистка списка
    m_connections.erase(it);

    //взаимное отключение (если у второго порта есть подключение)
    auto& list = port->m_connections;
    auto f = std::find(list.begin(), list.end(), this);
    if (f != list.end()) {
        port->m_connections.erase(f);
    }

    return true;
}

//--------------------------------------------------------
void PortOutput::detachAll()
{
    std::lock_guard<std::mutex> locker(m_mutex);
    _detachAll();
}

//--------------------------------------------------------
std::vector<IPortInput*> PortOutput::connections()
{
    std::lock_guard<std::mutex> locker(m_mutex);
    return m_connections;
}

//--------------------------------------------------------
bool PortOutput::_portCompatibility(PortInput* port)
{
    for (auto f1 : port->m_formats) {
        for (auto f2 : m_formats) {
            if (f1 == f2) {
                return true;
            }
        }
    }

    return false;
}

//--------------------------------------------------------
void PortOutput::_detachAll()
{
    for (IPortInput* port : m_connections) {
        port->detach(this);
    }
}
