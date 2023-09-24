#pragma once

#include <sys-monitor/i_cpu-monitor.h>

#include <atomic>
#include <mutex>

class CpuMonitorWin : public ICpuMonitor
{
public:
    CpuMonitorWin();
    virtual ~CpuMonitorWin();
    int current() const final;

private:
    void exec();
    std::atomic_int m_current;
    std::atomic_bool m_bexec;
    std::mutex m_mutex;
};
