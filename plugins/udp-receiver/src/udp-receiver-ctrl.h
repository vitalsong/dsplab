#pragma once

#include <QDialog>

#include <dsplab/controller.h>

namespace Ui {
class MainWindow;
}

namespace DspLab {

class UdpReceiverDev;
class UdpReceiverCtrl
  : public QDialog
  , public Controller
{
    Q_OBJECT

public:
    UdpReceiverCtrl();
    virtual ~UdpReceiverCtrl();

    QWidget* window() final;
    int valuesCount() const final;
    std::vector<DisplayValue> valuesList() const final;
    QVariantMap paramList() const final;
    void setParam(QVariantMap param) final;

private:
    void refreshUi();

private:
    Ui::MainWindow* ui;
    UdpReceiverDev* m_receiver;

private slots:
    void onConnectionListUpdated();
    void onUiPortChanged();
    void onUiAddressBoxChanged(int index);
    void onUiConnectionButtonClicked(bool enabled);
    void onUiClearButtonClicked();

private slots:
    void onDeviceStarted();
    void onDeviceStopped();
};

}   // namespace DspLab
