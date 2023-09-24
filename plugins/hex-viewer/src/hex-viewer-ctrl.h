#pragma once

#include <QDialog>

#include <dsplab/controller.h>

namespace Ui {
class IOHexVewierCtrl;
}

namespace DspLab {

class HexViewer;
class HexVewierCtrl
  : public QDialog
  , public Controller
{
    Q_OBJECT
public:
    HexVewierCtrl();
    virtual ~HexVewierCtrl();
    QWidget* window() final;

private:
    void showEvent(QShowEvent* e);
    void hideEvent(QHideEvent* e);

    void initForm();
    void clearViewData();

private:
    Ui::IOHexVewierCtrl* ui;
    HexViewer* m_device;

private slots:
    void onViewDataLines(QStringList listHex, QStringList listText);
    void onCodecData(int index);

    void onDeviceStarted();
    void onDeviceStopped();
};

}   // namespace DspLab
