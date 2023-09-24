#pragma once

#include <atomic>
#include <functional>
#include <thread>

namespace DspLab {

//warning: нельзя удалять таймер в функции обработчике!
class Timer
{
public:
    Timer(int period_ms, std::function<void()> fn);
    virtual ~Timer();

private:
    std::thread m_thread;
    std::atomic_bool m_started;
};

}   // namespace DspLab
