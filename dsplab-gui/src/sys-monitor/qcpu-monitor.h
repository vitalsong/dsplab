#pragma once

#include <QObject>

class QTimer;
class CpuMonitor;

class QCpuMonitor : public QObject
{
    Q_OBJECT

public:
    explicit QCpuMonitor(QObject* parent = nullptr);
    virtual ~QCpuMonitor();

    void setInterval(int secs);
    int getInterval() const;

    void start();
    void stop();

signals:
    void valueChanged(int percent);
    void textChanged(QString msg);

private slots:
    void onTimerTimeout();

private:
    CpuMonitor* m_monitor;
    QTimer* m_timer;
    int m_timeout;
};
