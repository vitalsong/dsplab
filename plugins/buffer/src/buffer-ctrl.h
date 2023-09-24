#pragma once

#include <QDialog>

#include <dsplab/controller.h>

namespace Ui {
class MainWindow;
}

namespace DspLab {

class BufferDev;
class BufferCtrl
  : public QDialog
  , public Controller
{
    Q_OBJECT

public:
    BufferCtrl();
    virtual ~BufferCtrl();
    QWidget* window() final;
    int valuesCount() const final;
    std::vector<DisplayValue> valuesList() const final;

private:
    Ui::MainWindow* ui;
    BufferDev* m_unit;

private slots:
    void onBufferSizeBoxChanged(int value);
};

}   // namespace DspLab
