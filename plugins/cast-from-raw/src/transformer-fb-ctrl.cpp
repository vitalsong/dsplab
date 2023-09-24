#include "transformer-fb-ctrl.h"
#include "transformer-fb.h"
#include "ui_main-window.h"

using namespace DspLab;

//------------------------------------------------------------------------------------------
TransformerFBCtrl::TransformerFBCtrl() :
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_unit = new TransformerFB();
    this->setWindowTitle("Cast (from raw)");
    this->setDeviceModel(m_unit);

    ui->middleFormatBox->addItem("int8", QVariant(TransformerFB::InInt8));
    ui->middleFormatBox->addItem("int16", QVariant(TransformerFB::InInt16));
    ui->middleFormatBox->addItem("int24", QVariant(TransformerFB::InInt24));
    ui->middleFormatBox->addItem("int32", QVariant(TransformerFB::InInt32));
    ui->middleFormatBox->addItem("int40", QVariant(TransformerFB::InInt40));

    //TODO: add little/big endian

    ui->outputFormatBox->addItem("real", QVariant(TransformerFB::OutReal));
    ui->outputFormatBox->addItem("complex", QVariant(TransformerFB::OutComplex));

    ui->middleFormatBox->setCurrentIndex(1);

    //настройка реакции на изменение формата промежуточного преобразования
    connect(ui->middleFormatBox, SIGNAL(activated(QString)),
            this, SLOT(onChangeMiddleFormat(QString)));

    //настройка реакции на изменение формата вывода
    connect(ui->outputFormatBox, SIGNAL(activated(QString)),
            this, SLOT(onChangeOutputFormat(QString)));
}

//------------------------------------------------------------------------------------------
TransformerFBCtrl::~TransformerFBCtrl()
{
    delete m_unit;
    delete ui;
}

//------------------------------------------------------------------------------------------
QWidget* TransformerFBCtrl::window()
{
    return this;
}

//------------------------------------------------------------------------------------------
int TransformerFBCtrl::valuesCount() const
{
    return 1;
}

//------------------------------------------------------------------------------------------
std::vector<DisplayValue> TransformerFBCtrl::valuesList() const
{
    QString text = ui->middleFormatBox->currentText() + QString(" -> ") + ui->outputFormatBox->currentText();
    return {{"Mode", text, DisplayValue::VALUE}};
}

//------------------------------------------------------------------------------------------
void TransformerFBCtrl::onChangeMiddleFormat(QString format)
{
    Q_UNUSED(format);
    int index = ui->middleFormatBox->currentIndex();
    auto mode = static_cast <TransformerFB::MidFormat> (ui->middleFormatBox->itemData(index).toInt());
    m_unit->setMidFormat(mode);
}

//------------------------------------------------------------------------------------------
void TransformerFBCtrl::onChangeOutputFormat(QString format)
{
    Q_UNUSED(format);
    int index = ui->middleFormatBox->currentIndex();
    auto mode = static_cast <TransformerFB::OutFormat> (ui->outputFormatBox->itemData(index).toInt());
    m_unit->setOutFormat(mode);
}
