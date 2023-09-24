#pragma once

#include <sys-monitor/i_cpu-monitor.h>

class CpuMonitor : public ICpuMonitor
{
public:
    CpuMonitor();
    virtual ~CpuMonitor();
    int current() const final;

private:
    ICpuMonitor* m_impl;
};
