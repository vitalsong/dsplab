#include <awgn-ctrl.h>
#include <awgn-dev.h>

#include "ui_main-window.h"

using namespace DspLab;

//-------------------------------------------------------------------------------------------------
AwgnCtrl::AwgnCtrl()
  : ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_unit = new AwgnDev();
    QDialog::setWindowTitle("AWGN");
    Controller::setDeviceModel(m_unit);

    connect(ui->snrSpinBox, SIGNAL(valueChanged(int)), this, SLOT(onSnrValueBoxChanged(int)));
    connect(ui->alphaSpinBox, SIGNAL(valueChanged(double)), this, SLOT(onExpCoeffBoxChanged(double)));
}

//-------------------------------------------------------------------------------------------------
AwgnCtrl::~AwgnCtrl()
{
    delete m_unit;
    delete ui;
}

//-------------------------------------------------------------------------------------------------
QWidget* AwgnCtrl::window()
{
    return this;
}

//-------------------------------------------------------------------------------------------------
QVariantMap AwgnCtrl::paramList() const
{
    QVariantMap list;
    list["awgn"] = m_unit->getSnr();
    list["smooth"] = m_unit->getExpCoeff();
    return list;
}

//-------------------------------------------------------------------------------------------------
void AwgnCtrl::setParam(QVariantMap param)
{
    m_unit->setSnr(param["awgn"].toDouble());
    m_unit->setExpCoeff(param["smooth"].toDouble());
    onRefreshUi();
}

//-------------------------------------------------------------------------------------------------
int AwgnCtrl::valuesCount() const
{
    return 1;
}

//-------------------------------------------------------------------------------------------------
std::vector<DisplayValue> DspLab::AwgnCtrl::valuesList() const
{
    return {{"SNR", m_unit->getSnr(), 2}};
}

//-------------------------------------------------------------------------------------------------
void AwgnCtrl::onSnrValueBoxChanged(int value)
{
    m_unit->setSnr(value);
}

//-------------------------------------------------------------------------------------------------
void AwgnCtrl::onExpCoeffBoxChanged(double value)
{
    m_unit->setExpCoeff(value);
}

//-------------------------------------------------------------------------------------------------
void AwgnCtrl::onRefreshUi()
{
    ui->snrSpinBox->setValue(m_unit->getSnr());
    ui->alphaSpinBox->setValue(m_unit->getExpCoeff());
}
