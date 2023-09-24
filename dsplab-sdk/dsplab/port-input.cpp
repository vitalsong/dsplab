#include "port-input.h"
#include "port-output.h"

#include "i_device.h"

#include <algorithm>
#include <QtCore>

using namespace DspLab;

//-------------------------------------------------------------------------------------------------
PortInput::PortInput(IDevice* parent)
{
    m_parent = parent;
}

//-------------------------------------------------------------------------------------------------
PortInput::~PortInput()
{
    std::lock_guard<std::mutex> locker(m_mutex);
    _detachAll();
}

//-------------------------------------------------------------------------------------------------
IDevice* PortInput::parent() const
{
    return m_parent;
}

//-------------------------------------------------------------------------------------------------
std::vector<Format> PortInput::avaliableFormats() const
{
    std::lock_guard<std::mutex> locker(m_mutex);
    return m_formats;
}

//-------------------------------------------------------------------------------------------------
void PortInput::setAvaliableFormats(std::vector<Format> format)
{
    std::lock_guard<std::mutex> locker(m_mutex);
    m_formats = format;
    _clearBuffer();
}

//-------------------------------------------------------------------------------------------------
void PortInput::frameRead(Frame& frame)
{
    std::lock_guard<std::mutex> locker(m_mutex);
    if (m_buffer.size() > 0) {
        frame = m_buffer.front();
        m_buffer.pop_front();
    }
}

//-------------------------------------------------------------------------------------------------
void PortInput::frameClear()
{
    std::lock_guard<std::mutex> locker(m_mutex);
    _clearBuffer();
}

//-------------------------------------------------------------------------------------------------
void PortInput::write(const Frame& frame)
{
    std::lock_guard<std::mutex> locker(m_mutex);

    //если устройство родитель активно
    if (m_parent->started()) {
        //если формат фрейма соответствует
        if (_formatSupported(frame.format())) {
            m_buffer.push_back(frame);
        } else {
            //сообщаем о несоответствии формата
            qWarning() << "Данный формат не поддерживается";

            //останавливаем "родителя"
            //ЗЫ: не лучшее решение, устройство в идеале должно
            //принимать решение об отключении на основе события
            this->parent()->stop();
        }
    }
}

//-------------------------------------------------------------------------------------------------
int PortInput::frameCount() const
{
    std::lock_guard<std::mutex> locker(m_mutex);
    return m_buffer.size();
}

//-------------------------------------------------------------------------------------------------
bool PortInput::attach(IPortOutput* _port)
{
    std::lock_guard<std::mutex> locker(m_mutex);
    PortOutput* port = dynamic_cast<PortOutput*>(_port);

    //если есть обратная связь
    if (m_parent == port->m_parent) {
        return false;
    }

    //если уже существует хотя бы одно подключение
    if (!m_connections.empty()) {
        return false;
    }

    //если порты не совместимы
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
    const auto& list = port->m_connections;
    if (std::find(list.begin(), list.end(), this) == list.end()) {
        port->m_connections.push_back(this);
        return true;
    }

    return false;
}

//-------------------------------------------------------------------------------------------------
bool PortInput::detach(IPortOutput* _port)
{
    std::lock_guard<std::mutex> locker(m_mutex);
    PortOutput* port = dynamic_cast<PortOutput*>(_port);

    auto it = std::find(m_connections.begin(), m_connections.end(), port);
    if (it == m_connections.end()) {
        return false;
    }

    //удаление из собственного списка подключений
    m_connections.erase(it);

    //взаимное отключение (если у второго порта есть подключение с нами)
    auto& list = port->m_connections;
    auto f = std::find(list.begin(), list.end(), this);
    if (f != list.end()) {
        port->m_connections.erase(f);
    }

    return true;
}

//-------------------------------------------------------------------------------------------------
void PortInput::detachAll()
{
    std::lock_guard<std::mutex> locker(m_mutex);
    _detachAll();
}

//-------------------------------------------------------------------------------------------------
std::vector<IPortOutput*> PortInput::connections()
{
    std::lock_guard<std::mutex> locker(m_mutex);
    return m_connections;
}

//-------------------------------------------------------------------------------------------------
bool PortInput::_formatSupported(const Format& format)
{
    for (const Format& it : m_formats) {
        if (it == format) {
            return true;
        }
    }

    return false;
}

//-------------------------------------------------------------------------------------------------
bool PortInput::_portCompatibility(PortOutput* port)
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

//-------------------------------------------------------------------------------------------------
void PortInput::_detachAll()
{
    for (IPortOutput* port : m_connections) {
        port->detach(this);
    }
}

//-------------------------------------------------------------------------------------------------
void PortInput::_clearBuffer()
{
    m_buffer.clear();
}
