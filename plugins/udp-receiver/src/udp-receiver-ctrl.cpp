#include "udp-receiver-ctrl.h"
#include "ui_main-window.h"
#include "udp-receiver-dev.h"

using namespace DspLab;

//-------------------------------------------------------------------------------------------------
UdpReceiverCtrl::UdpReceiverCtrl()
  : ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_receiver = new UdpReceiverDev();
    this->setWindowTitle("UDP Receiver");
    this->setDeviceModel(m_receiver);

    //ответ от приемника об изменениий состояния
    connect(m_receiver, SIGNAL(sgConnectionListUpdated()), this, SLOT(onConnectionListUpdated()));

    //выбор входного направления
    connect(ui->addressBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onUiAddressBoxChanged(int)));

    //смена порта приема
    connect(ui->portBox, SIGNAL(editingFinished()), this, SLOT(onUiPortChanged()));

    //нажатие кнопки подключения
    connect(ui->connectionButton, SIGNAL(clicked(bool)), this, SLOT(onUiConnectionButtonClicked(bool)));

    //очистка списка подключений
    connect(ui->clearAddressList, SIGNAL(clicked(bool)), this, SLOT(onUiClearButtonClicked()));

    //реакция от устройства
    connect(m_receiver, SIGNAL(deviceStarted()), this, SLOT(onDeviceStarted()));
    connect(m_receiver, SIGNAL(deviceStopped()), this, SLOT(onDeviceStopped()));
}

//-------------------------------------------------------------------------------------------------
UdpReceiverCtrl::~UdpReceiverCtrl()
{
    delete m_receiver;
    delete ui;
}

//-------------------------------------------------------------------------------------------------
QWidget* UdpReceiverCtrl::window()
{
    return this;
}

//-------------------------------------------------------------------------------------------------
int UdpReceiverCtrl::valuesCount() const
{
    return 2;
}

//-------------------------------------------------------------------------------------------------
std::vector<DisplayValue> UdpReceiverCtrl::valuesList() const
{
    std::vector<DisplayValue> list;
    auto addr = m_receiver->addressFilter();
    auto ip = QHostAddress(addr.first.toIPv4Address());
    list.push_back({"ip", ip.toString(), DisplayValue::VALUE});
    list.push_back({"port", QString::number(addr.second), DisplayValue::VALUE});
    return list;
}

//-------------------------------------------------------------------------------------------------
QVariantMap UdpReceiverCtrl::paramList() const
{
    QVariantMap list;
    list["port"] = m_receiver->bindPort();
    return list;
}

//-------------------------------------------------------------------------------------------------
void UdpReceiverCtrl::setParam(QVariantMap param)
{
    m_receiver->setBindPort(param["port"].toInt());
    refreshUi();
}

//-------------------------------------------------------------------------------------------------
void UdpReceiverCtrl::refreshUi()
{
    ui->portBox->setValue(m_receiver->bindPort());
}

//-------------------------------------------------------------------------------------------------
void UdpReceiverCtrl::onUiPortChanged()
{
    quint16 port = ui->portBox->value();
    m_receiver->setBindPort(port);
}

//-------------------------------------------------------------------------------------------------
void UdpReceiverCtrl::onUiAddressBoxChanged(int index)
{
    std::vector<UdpReceiverDev::Address> addr = m_receiver->connectionList();
    if (!addr.empty() && (index >= 0)) {
        m_receiver->setAddressFilter(addr[index]);
    }
}

//-------------------------------------------------------------------------------------------------
void UdpReceiverCtrl::onConnectionListUpdated()
{
    ui->addressBox->clear();

    std::vector<UdpReceiverDev::Address> list = m_receiver->connectionList();
    for (UdpReceiverDev::Address& it : list) {
        QString ip = it.first.toString();
        QString port = QString::number(it.second);
        ui->addressBox->addItem(ip + tr(" - ") + port);
    }
}

//-------------------------------------------------------------------------------------------------
void UdpReceiverCtrl::onUiConnectionButtonClicked(bool enabled)
{
    if (enabled) {
        m_receiver->start();
    } else {
        m_receiver->stop();
    }
}

//-------------------------------------------------------------------------------------------------
void UdpReceiverCtrl::onUiClearButtonClicked()
{
    m_receiver->refresh();
}

//-------------------------------------------------------------------------------------------------
void UdpReceiverCtrl::onDeviceStarted()
{
    ui->portBox->setEnabled(false);
}

//-------------------------------------------------------------------------------------------------
void UdpReceiverCtrl::onDeviceStopped()
{
    ui->portBox->setEnabled(true);
}
