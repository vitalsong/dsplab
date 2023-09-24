#include <sys-monitor/cpu-monitor.h>

#ifdef __MINGW32__
#include "cpu-monitor-win.h"
#else
#include "cpu-monitor-linux.h"
#endif

//------------------------------------------------------------
CpuMonitor::CpuMonitor()
{
#ifdef __MINGW32__
    m_impl = new CpuMonitorWin();
#else
    m_impl = new CpuMonitorLinux();
#endif
}

//------------------------------------------------------------
CpuMonitor::~CpuMonitor()
{
    delete m_impl;
}

//------------------------------------------------------------
int CpuMonitor::current() const
{
    return m_impl->current();
}
