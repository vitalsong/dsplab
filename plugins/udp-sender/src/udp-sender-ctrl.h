#pragma once

#include <QDialog>
#include <dsplab/controller.h>

namespace Ui {
class MainWindow;
}

namespace DspLab {

class UdpSenderDev;
class UdpSenderCtrl
  : public QDialog
  , public Controller
{
    Q_OBJECT

public:
    UdpSenderCtrl();
    virtual ~UdpSenderCtrl();
    QWidget* window() final;
    QVariantMap paramList() const override;
    void setParam(QVariantMap param) override;

private:
    void refreshUi();

private:
    Ui::MainWindow* ui;
    UdpSenderDev* m_sender;

private slots:
    /*!
     * \brief onCreateTransmit
     * создания нового передатчика
     */
    void onCreateTransmit();

    /*!
     * \brief onDestroyTransmit
     * удаления передатчика
     */
    void onDestroyTransmit();

    /*!
     * \brief onCustomContextMenuRequested
     * контекстное меню
     */
    void onCustomContextMenuRequested(QPoint);
};

}   // namespace DspLab
