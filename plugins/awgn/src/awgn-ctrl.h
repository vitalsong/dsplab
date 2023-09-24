#pragma once

#include <QDialog>

#include <dsplab/controller.h>

namespace Ui {
class MainWindow;
}

namespace DspLab {

class AwgnDev;
class AwgnCtrl
  : public QDialog
  , public Controller
{
    Q_OBJECT

public:
    AwgnCtrl();
    virtual ~AwgnCtrl();
    QWidget* window() final;

    //сохранение конфигурации
    QVariantMap paramList() const final;
    void setParam(QVariantMap param) final;

    //параметры для отображения
    int valuesCount() const final;
    std::vector<DisplayValue> valuesList() const final;

private:
    Ui::MainWindow* ui;
    AwgnDev* m_unit;

private slots:
    void onSnrValueBoxChanged(int value);
    void onExpCoeffBoxChanged(double value);
    void onRefreshUi();
};

}   // namespace DspLab
