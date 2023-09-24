#include <udp-sender-ctrl.h>
#include <udp-sender-dev.h>
#include "ui_main-window.h"

#include <QMenu>
#include <QAction>

using namespace DspLab;

//-------------------------------------------------------------------------------------------------
UdpSenderCtrl::UdpSenderCtrl()
  : ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_sender = new UdpSenderDev();
    this->setWindowTitle("UDP Sender");
    this->setDeviceModel(m_sender);

    //Ргулярное выражение для проверки IP
    QString ipRange1 = "(?:[0-1]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])";
    QString ipRange2 = "(?:[0-1]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])";
    QString ipRange3 = "(?:[1-1]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])";
    QString ipRange4 = "(?:[1-1]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])";

    QRegExp ipRegex("^" + ipRange1 + "\\." + ipRange2 + "\\." + ipRange3 + "\\." + ipRange4 + "$");
    QRegExpValidator* ipValidation = new QRegExpValidator(ipRegex, this);
    ui->adrr->setValidator(ipValidation);

    ui->tableTransmits->horizontalHeader()->resizeSection(1, 10);
    ui->tableTransmits->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);

    connect(ui->addSender, SIGNAL(clicked()), this, SLOT(onCreateTransmit()));

    connect(ui->tableTransmits, SIGNAL(customContextMenuRequested(QPoint)), this,
            SLOT(onCustomContextMenuRequested(QPoint)));
}

//-------------------------------------------------------------------------------------------------
UdpSenderCtrl::~UdpSenderCtrl()
{
    delete m_sender;
    delete ui;
}

//-------------------------------------------------------------------------------------------------
QWidget* UdpSenderCtrl::window()
{
    return this;
}

//-------------------------------------------------------------------------------------------------
QVariantMap UdpSenderCtrl::paramList() const
{
    int num = 0;
    QVariantMap map;
    auto list = m_sender->transmitList();
    for (auto tr : list) {
        QString key;
        key = QString("addr_") + QString::number(num);
        map[key] = tr.addr.toString();
        key = QString("port_") + QString::number(num);
        map[key] = tr.port;
        num++;
    }

    return map;
}

//-------------------------------------------------------------------------------------------------
void UdpSenderCtrl::setParam(QVariantMap param)
{
    //настройка передатчика
    int n = param.size() / 2;
    for (int i = 0; i < n; ++i) {
        QString key;
        key = QString("addr_") + QString::number(i);
        QHostAddress addr = QHostAddress(param[key].toString());
        key = QString("port_") + QString::number(i);
        quint16 port = param[key].toInt();
        m_sender->addTransmit(UdpSenderDev::Transmit(addr, port));
    }

    //обновление графики
    refreshUi();
}

//-------------------------------------------------------------------------------------------------
void UdpSenderCtrl::refreshUi()
{
    auto list = m_sender->transmitList();

    ui->tableTransmits->clear();
    ui->tableTransmits->setRowCount(list.size());

    int irow = 0;
    for (auto tr : list) {
        QString addr = tr.addr.toString();
        QString port = QString::number(tr.port);
        ui->tableTransmits->setItem(irow, 0, new QTableWidgetItem(addr));
        ui->tableTransmits->setItem(irow, 1, new QTableWidgetItem(port));
        ++irow;
    }
}

//-------------------------------------------------------------------------------------------------
void UdpSenderCtrl::onCreateTransmit()
{
    QString addr = ui->adrr->text();
    QString port = ui->port->text();

    if (addr.isEmpty() || port.isEmpty()) {
        return;
    }

    UdpSenderDev::Transmit transmit(QHostAddress(addr), port.toInt());
    m_sender->addTransmit(transmit);
    refreshUi();
}

//-------------------------------------------------------------------------------------------------
void UdpSenderCtrl::onDestroyTransmit()
{
    int irow = ui->tableTransmits->currentIndex().row();
    if (irow < 0) {
        return;
    }

    QHostAddress addr = QHostAddress(ui->tableTransmits->item(irow, 0)->text());
    int port = ui->tableTransmits->item(irow, 1)->text().toInt();
    UdpSenderDev::Transmit tr(addr, port);
    m_sender->delTransmit(tr);
    refreshUi();
}

//-------------------------------------------------------------------------------------------------
void UdpSenderCtrl::onCustomContextMenuRequested(QPoint pos)
{
    QMenu menu;
    QAction* deleteTransmit = menu.addAction("Удалить");

    //текущее выбранное действие
    QAction* currentAction = menu.exec(ui->tableTransmits->mapToGlobal(pos));

    //обработка выбора меню
    if (currentAction == deleteTransmit) {
        this->onDestroyTransmit();
    }
}
