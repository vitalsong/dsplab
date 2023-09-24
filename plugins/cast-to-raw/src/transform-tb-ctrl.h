#pragma once

#include <QDialog>

#include <dsplab/controller.h>

namespace Ui {
class MainWindow;
}

namespace DspLab {

class TransformTB;
class TransformTBCtrl
  : public QDialog
  , public Controller
{
    Q_OBJECT

public:
    TransformTBCtrl();
    virtual ~TransformTBCtrl();
    QWidget* window();
    int valuesCount() const final;
    std::vector<DisplayValue> valuesList() const final;

private:
    Ui::MainWindow* ui;
    TransformTB* m_unit;

private slots:
    void onChangeMidFormat(QString format);
};

}   // namespace DspLab
