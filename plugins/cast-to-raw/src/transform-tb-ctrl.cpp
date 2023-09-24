#include "transform-tb-ctrl.h"
#include "transform-tb.h"
#include "ui_main-window.h"

using namespace DspLab;

//------------------------------------------------------------------------------------------
TransformTBCtrl::TransformTBCtrl()
  : ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_unit = new TransformTB();
    this->setWindowTitle("Cast (to raw)");
    this->setDeviceModel(m_unit);

    //настройка реакции на изменение формата промежуточного преобразования
    connect(ui->middleFormatBox, SIGNAL(activated(QString)), this, SLOT(onChangeMidFormat(QString)));
}

//------------------------------------------------------------------------------------------
TransformTBCtrl::~TransformTBCtrl()
{
    delete m_unit;
    delete ui;
}

//------------------------------------------------------------------------------------------
QWidget* TransformTBCtrl::window()
{
    return this;
}

//------------------------------------------------------------------------------------------
int TransformTBCtrl::valuesCount() const
{
    return 1;
}

//------------------------------------------------------------------------------------------
std::vector<DisplayValue> TransformTBCtrl::valuesList() const
{
    QString text = ui->middleFormatBox->currentText() + QString(" -> bytes");
    return {{"Mode", text, DisplayValue::VALUE}};
}

//------------------------------------------------------------------------------------------
void TransformTBCtrl::onChangeMidFormat(QString format)
{
    if (format == "int8") {
        m_unit->setIntegerFormat(TransformTB::MID_INT8);
    } else if (format == "int16") {
        m_unit->setIntegerFormat(TransformTB::MID_INT16);
    } else if (format == "int24") {
        m_unit->setIntegerFormat(TransformTB::MID_INT24);
    } else if (format == "int32") {
        m_unit->setIntegerFormat(TransformTB::MID_INT32);
    } else if (format == "int40") {
        m_unit->setIntegerFormat(TransformTB::MID_INT40);
    }
}
