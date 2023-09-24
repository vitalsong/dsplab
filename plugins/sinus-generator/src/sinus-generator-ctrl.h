#pragma once

#include <QDialog>
#include <dsplab/controller.h>

namespace Ui {
class MainWindow;
}

namespace DspLab {

class SinusGenerator;

/*!
 * \brief Контроллер для генератора синуса
 */
class SinusGeneratorCtrl
  : public QDialog
  , public Controller
{
    Q_OBJECT

public:
    SinusGeneratorCtrl();
    virtual ~SinusGeneratorCtrl();
    QVariantMap paramList() const final;
    void setParam(QVariantMap list) final;
    QWidget* window() final;

private:
    Ui::MainWindow* ui;
    SinusGenerator* m_singen;

private slots:
    //готовность данных в полях UI
    void onSampleRateEditingFinished();
    void onFrequensyEditingFinished();
    void onAmplitudeEditingFinished();
    void onFullScaleEditingFinished();
    void onGeneratePeriodEditingFinished();

    //обновление UI
    void onRefreshParamUi();

    //контроль активности польз.интерфейса
    void onEnableUi();
    void onDisableUi();
};

}   // namespace DspLab
