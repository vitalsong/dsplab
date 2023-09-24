#pragma once

#include <QDialog>

#include <dsplab/controller.h>

namespace Ui {
class MainWindow;
}

namespace DspLab {

class TransformerFB;
class TransformerFBCtrl
  : public QDialog
  , public Controller
{
    Q_OBJECT

public:
    TransformerFBCtrl();
    virtual ~TransformerFBCtrl();
    QWidget* window() final;
    int valuesCount() const final;
    std::vector<DisplayValue> valuesList() const final;

private:
    Ui::MainWindow* ui;
    TransformerFB* m_unit;

private slots:
    void onChangeMiddleFormat(QString format);
    void onChangeOutputFormat(QString format);
};

}   // namespace DspLab
