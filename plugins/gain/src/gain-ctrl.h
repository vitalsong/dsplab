#pragma once

#include <QDialog>

#include <dsplab/controller.h>

namespace Ui {
class MainWindow;
}

namespace DspLab {

class Gain;
class GainCtrl
  : public QDialog
  , public Controller
{
    Q_OBJECT

public:
    GainCtrl();
    virtual ~GainCtrl();
    QWidget* window() final;

    int valuesCount() const final;
    std::vector<DisplayValue> valuesList() const final;

private:
    Ui::MainWindow* ui;
    Gain* m_gain;

private slots:
    void onGainDialChanged(int gain);
};

}   // namespace DspLab
