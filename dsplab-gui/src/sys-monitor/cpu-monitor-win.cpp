#include <sys-monitor/cpu-monitor-win.h>

#include <functional>
#include <windows.h>
#include <thread>

//------------------------------------------------------------
CpuMonitorWin::CpuMonitorWin()
{
    m_bexec = true;
    m_current = 0;

    std::thread th(std::bind(&CpuMonitorWin::exec, this));
    th.detach();
}

//------------------------------------------------------------
CpuMonitorWin::~CpuMonitorWin()
{
    m_bexec = false;
    std::lock_guard<std::mutex> locker(m_mutex);
}

//------------------------------------------------------------
int CpuMonitorWin::current() const
{
    return m_current;
}

//------------------------------------------------------------
static __int64 fileTimeToInt64(FILETIME& f)
{
    ULARGE_INTEGER time;
    time.LowPart = f.dwLowDateTime;
    time.HighPart = f.dwHighDateTime;
    return time.QuadPart;
}

//------------------------------------------------------------
void CpuMonitorWin::exec()
{
    std::lock_guard<std::mutex> locker(m_mutex);

    HANDLE hProcess = GetCurrentProcess();

    FILETIME creationTime;
    FILETIME exitTime;
    FILETIME kernelTime;
    FILETIME userTime;
    FILETIME iddleTime;

    __int64 processKernelTime;
    __int64 processUserTime;
    __int64 processTotalTime;
    __int64 systemKernelTime;
    __int64 systemUserTime;
    __int64 systemTotalTime;

    //промежутки времени
    __int64 dProcessTotalTime;
    __int64 dSystemTotalTime;

    //прошлые значения
    __int64 lastProcessTotalTime = 0;
    __int64 lastSystemTotalTime = 0;

    while (m_bexec) {
        //время процесса
        ::GetProcessTimes(hProcess, &creationTime, &exitTime, &kernelTime, &userTime);
        processKernelTime = fileTimeToInt64(kernelTime);
        processUserTime = fileTimeToInt64(userTime);
        processTotalTime = processKernelTime + processUserTime;
        dProcessTotalTime = processTotalTime - lastProcessTotalTime;
        lastProcessTotalTime = processTotalTime;

        //время системы
        ::GetSystemTimes(&iddleTime, &kernelTime, &userTime);
        systemKernelTime = fileTimeToInt64(kernelTime);
        systemUserTime = fileTimeToInt64(userTime);
        systemTotalTime = systemKernelTime + systemUserTime;
        dSystemTotalTime = systemTotalTime - lastSystemTotalTime;
        lastSystemTotalTime = systemTotalTime;

        //расчет нагрузки
        double load = double(dProcessTotalTime) / double(dSystemTotalTime) * 100;
        m_current = load + 0.5f;

        //ожидание в 1 секунду
        ::Sleep(1000);
    }
}
