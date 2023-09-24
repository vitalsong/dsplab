#include <logging/logger.h>

#include <QDateTime>

static QListWidget* g_widget = nullptr;

//-------------------------------------------------------------------------------------------------
void Logger::instance(QListWidget* w)
{
    g_widget = w;
}

//-------------------------------------------------------------------------------------------------
static void sWriteDefaultMsg(QString str)
{
    QListWidgetItem* item = new QListWidgetItem(g_widget);
    item->setBackgroundColor(Qt::white);
    item->setText(str);
    g_widget->addItem(item);
    g_widget->scrollToBottom();
}

//-------------------------------------------------------------------------------------------------
static void sWriteErrorMsg(QString str)
{
    QListWidgetItem* item = new QListWidgetItem(g_widget);
    item->setBackgroundColor(Qt::red);
    item->setText(str);
    g_widget->addItem(item);
    g_widget->scrollToBottom();
}

//-------------------------------------------------------------------------------------------------
static void sWriteWarningMsg(QString str)
{
    QListWidgetItem* item = new QListWidgetItem(g_widget);
    item->setBackgroundColor(Qt::yellow);
    item->setText(str);
    g_widget->addItem(item);
    g_widget->scrollToBottom();
}

//-------------------------------------------------------------------------------------------------
void Logger::print(Logger::EventType type, QString msg)
{
    QTime time = QTime::currentTime();
    QString text = time.toString() + ": " + msg;

    switch (type) {
    case Info:
        sWriteDefaultMsg(text);
        break;

    case Warning:
        sWriteWarningMsg(text);
        break;

    case Error:
        sWriteErrorMsg(text);
        break;
    }
}
