#include <sys-monitor/qcpu-monitor.h>
#include <sys-monitor/cpu-monitor.h>
#include <qtimer.h>

//--------------------------------------------------------------
QCpuMonitor::QCpuMonitor(QObject* parent)
  : QObject(parent)
{
    m_monitor = new CpuMonitor();
    m_timer = new QTimer();
    m_timeout = 1;

    connect(m_timer, SIGNAL(timeout()), this, SLOT(onTimerTimeout()));
}

//--------------------------------------------------------------
QCpuMonitor::~QCpuMonitor()
{
    delete m_timer;
    delete m_monitor;
}

//--------------------------------------------------------------
void QCpuMonitor::setInterval(int secs)
{
    m_timeout = secs;
}

//--------------------------------------------------------------
int QCpuMonitor::getInterval() const
{
    return m_timeout;
}

//--------------------------------------------------------------
void QCpuMonitor::start()
{
    m_timer->start(m_timeout * 1000);
}

//--------------------------------------------------------------
void QCpuMonitor::stop()
{
    m_timer->stop();
}

//--------------------------------------------------------------
void QCpuMonitor::onTimerTimeout()
{
    int percent = m_monitor->current();
    emit valueChanged(percent);
    emit textChanged(QString("CPU: %1").arg(percent));
}
