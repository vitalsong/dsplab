#pragma once

#include <QDialog>

#include <dsplab/controller.h>

namespace Ui {
class MainWindow;
}

namespace DspLab {

class NoiseGenerator;
class NoiseGeneratorCtrl
  : public QDialog
  , public Controller
{
    Q_OBJECT

public:
    NoiseGeneratorCtrl();
    virtual ~NoiseGeneratorCtrl();
    QWidget* window() final;
    int valuesCount() const final;
    std::vector<DisplayValue> valuesList() const final;
    QVariantMap paramList() const final;
    void setParam(QVariantMap param) final;

private:
    void refreshUi();

private:
    Ui::MainWindow* ui;
    NoiseGenerator* m_gen;

private slots:
    void onChangeExpectationBox(double value);
    void onChangeDeviationBox(double value);
    void onChangeGenPeriodBox(int msec);
    void onChangeSampleRateBox(int hz);
    void onComplexTypeBoxChecked(bool checked);

private slots:
    void onDeviceStarted();
    void onDeviceStopped();
};

}   // namespace DspLab
