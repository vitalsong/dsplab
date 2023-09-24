#include "device.h"
#include "port-input.h"
#include "port-output.h"
#include "timer/timer.h"

#include <chrono>
#include <thread>
#include <unistd.h>
#include <assert.h>
#include <functional>

#include <QtCore>
#include <QTimer>

using namespace DspLab;

//-------------------------------------------------------------------------------------------
Device::Device(int nin, int nout)
  : m_started(false)
  , m_activate_counter(0)
  , m_standby(false)
  , m_standby_msec(1000)
{
    //входной набор
    for (int i = 0; i < nin; ++i) {
        m_inputs.push_back(new PortInput(this));
    }

    //выходной набор
    for (int i = 0; i < nout; ++i) {
        m_outputs.push_back(new PortOutput(this));
    }

    //настройка таймера для периодического вызова обработчика
    m_timeout = 0;
    m_use_timer = false;
    m_last_active = std::chrono::system_clock::now();
}

//-------------------------------------------------------------------------------------------
Device::~Device()
{
    m_timer.reset();
    m_started = false;

    //ждем завершения фонового потока обработки
    std::lock_guard<std::mutex> locker(m_exec_lock);

    //удаляем объекты портов
    //TODO: это может быть проблемой при удалении нескольких устройств
    for (PortInput* port : m_inputs) {
        delete port;
    }

    for (PortOutput* port : m_outputs) {
        delete port;
    }
}

//-------------------------------------------------------------------------------------------
IPortInput* Device::input(int num) const
{
    return m_inputs.at(num);
}

//-------------------------------------------------------------------------------------------
IPortOutput* Device::output(int num) const
{
    return m_outputs.at(num);
}

//-------------------------------------------------------------------------------------------
int Device::num_inputs() const
{
    return m_inputs.size();
}

//-------------------------------------------------------------------------------------------
int Device::num_outputs() const
{
    return m_outputs.size();
}

//-------------------------------------------------------------------------------------------
bool Device::started() const
{
    return m_started;
}

//-------------------------------------------------------------------------------------------
bool Device::standby() const
{
    return m_standby;
}

//-------------------------------------------------------------------------------------------
void Device::activate()
{
    //если устройство запущено
    if (m_started) {
        std::unique_lock<std::mutex> locker(m_exec_lock);

        //наращиваем счетчик активации
        ++m_activate_counter;

        //освобождаем поток
        m_activate_check.notify_one();
    }
}

//-------------------------------------------------------------------------------------------
void Device::start()
{
    if (!m_started) {
        m_last_active = std::chrono::system_clock::now();

        //статус запуска (для потока)
        m_started = true;

        //создаем основной обработчик (для устройств "не истоков")
        if (!m_use_timer) {
            //запуск стандартного цикла обработки
            std::thread th([this] {
                thread_execute();
            });
            th.detach();
        } else {
            //запуск цикла обработки по таймеру
            if (!m_timer) {
                m_timer = std::make_shared<Timer>(m_timeout, [this] {
                    timer_execute();
                });
            }
        }

        //сброс параметров ожидания
        m_standby = false;

        //сообщаем, что устройство стартовало
        emit deviceStarted();
    }
}

//-------------------------------------------------------------------------------------------
void Device::stop()
{
    if (m_started) {
        //статус запуска (для потока)
        m_started = false;

        //очистка входных буферов
        for (PortInput* port : m_inputs) {
            port->frameClear();
        }

        //сообщаем, что устройство было остановлено
        //ЗЫ: а если вызов произведен при удалении?
        emit deviceStopped();
    }
}

//-------------------------------------------------------------------------------------------
void Device::ioWrite(const Frame& frame, int numSelfOut)
{
    PortOutput* port = m_outputs[numSelfOut];
    std::vector<IPortInput*> connections = port->connections();

    //запись и активация
    for (IPortInput* it : connections) {
        it->write(frame);
        it->parent()->activate();
    }
}

//-------------------------------------------------------------------------------------------
void Device::ioRead(Frame& frame, int numSelfIn) const
{
    PortInput* port = m_inputs[numSelfIn];
    port->frameRead(frame);
}

//-------------------------------------------------------------------------------------------
int Device::ioSize(int numSelfIn) const
{
    PortInput* port = m_inputs[numSelfIn];
    return port->frameCount();
}

//-------------------------------------------------------------------------------------------
void Device::ioClear(int numSelfIn)
{
    PortInput* port = m_inputs[numSelfIn];
    port->frameClear();
}

//-------------------------------------------------------------------------------------------
void Device::setAvaliableInputFormats(std::vector<Format> format, int numSelfIn)
{
    PortInput* port = m_inputs[numSelfIn];
    port->setAvaliableFormats(format);
}

//-------------------------------------------------------------------------------------------
void Device::setAvaliableOutputFormats(std::vector<Format> format, int numSelfOut)
{
    PortOutput* port = m_outputs[numSelfOut];
    port->setAvaliableFormats(format);
}

//-------------------------------------------------------------------------------------------
void Device::setProcessingTimeoutInterval(int msec, bool enabled)
{
    if (enabled) {
        m_use_timer = true;
        m_timeout = msec;
    } else {
        this->stop();
        m_use_timer = false;
        m_timeout = 0;
    }
}

//-------------------------------------------------------------------------------------------
void Device::getProcessingTimeoutInterval(int& msec, bool& enabled)
{
    msec = m_timeout;
    enabled = m_use_timer;
}

//-------------------------------------------------------------------------------------------
void Device::setStandbyTimeout(int msec)
{
    m_standby_msec = msec;
}

//-------------------------------------------------------------------------------------------
int Device::getStandbyTimeout() const
{
    return m_standby_msec;
}

//-------------------------------------------------------------------------------------------
void Device::thread_execute()
{
    while (m_started) {
        std::unique_lock<std::mutex> locker(m_exec_lock);

        //ждем, пока не освободится поток и счетчик активации не будет больше 0
        m_activate_check.wait(locker, [&]() {
            return (m_activate_counter > 0);
        });

        //ловим исключения в базовой функции обработки
        try {
            this->check_standby();
            this->processing();
        } catch (...) {
            qCritical() << "Неизвестная ошибка";
            this->stop();
        }

        m_activate_counter -= 1;
    }
}

//-------------------------------------------------------------------------------------------
void Device::timer_execute()
{
    //ловим исключения в базовой функции обработки
    if (m_started) {
        try {
            this->check_standby();
            this->processing();
        } catch (...) {
            qCritical() << "Неизвестная ошибка";
            this->stop();
        }
    }
}

//-------------------------------------------------------------------------------------------
void Device::check_standby()
{
    using namespace std::chrono;

    bool standby = m_standby;

    //не обрабатываем для устройств истоков
    if (m_inputs.size() == 0) {
        standby = true;
    } else {
        //если на входе имеется хотя бы один фрейм
        for (int i = 0; i < m_inputs.size(); ++i) {
            if (ioSize(i)) {
                m_last_active = system_clock::now();
                break;
            }
        }

        auto t1 = system_clock::now();
        auto dt = duration_cast<milliseconds>(t1 - m_last_active);
        if (dt.count() > m_standby_msec) {
            standby = true;
        } else {
            standby = false;
        }
    }

    //сообщаем только при изменении состояния
    if (standby != m_standby) {
        m_standby = standby;
        emit deviceStandby(m_standby);
    }
}
