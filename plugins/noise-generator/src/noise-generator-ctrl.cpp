#include <noise-generator-ctrl.h>
#include <noise-generator.h>
#include "ui_main-window.h"

using namespace DspLab;

//-------------------------------------------------------------------------------------------------
NoiseGeneratorCtrl::NoiseGeneratorCtrl()
  : ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_gen = new NoiseGenerator();
    this->setWindowTitle("Noise Generator");
    this->setDeviceModel(m_gen);

    //настройка сигналов от элементов UI
    connect(ui->expectansionValueBox, SIGNAL(valueChanged(double)), this, SLOT(onChangeExpectationBox(double)));

    connect(ui->deviationValueBox, SIGNAL(valueChanged(double)), this, SLOT(onChangeDeviationBox(double)));

    connect(ui->periodValueBox, SIGNAL(valueChanged(int)), this, SLOT(onChangeGenPeriodBox(int)));

    connect(ui->sampleRateValueBox, SIGNAL(valueChanged(int)), this, SLOT(onChangeSampleRateBox(int)));

    connect(ui->complexSignalBox, SIGNAL(clicked(bool)), this, SLOT(onComplexTypeBoxChecked(bool)));

    //обновление полей
    refreshUi();
}

//-------------------------------------------------------------------------------------------------
NoiseGeneratorCtrl::~NoiseGeneratorCtrl()
{
    delete m_gen;
    delete ui;
}

//-------------------------------------------------------------------------------------------------
QWidget* NoiseGeneratorCtrl::window()
{
    return this;
}

//-------------------------------------------------------------------------------------------------
int NoiseGeneratorCtrl::valuesCount() const
{
    return 3;
}

//-------------------------------------------------------------------------------------------------
std::vector<DisplayValue> NoiseGeneratorCtrl::valuesList() const
{
    std::vector<DisplayValue> list;
    list.push_back({"Rate", m_gen->sampleRate(), DisplayValue::NAME_VALUE_UNIT, "Hz"});
    list.push_back({"Exp", m_gen->expectation(), 2, DisplayValue::NAME_VALUE});
    list.push_back({"Dev", m_gen->deviation(), 2, DisplayValue::NAME_VALUE});
    return list;
}

//-------------------------------------------------------------------------------------------------
QVariantMap NoiseGeneratorCtrl::paramList() const
{
    QVariantMap list;
    list["expectation"] = m_gen->expectation();
    list["deviation"] = m_gen->deviation();
    list["genPeriod"] = m_gen->generatePeriod();
    list["sampleRate"] = m_gen->sampleRate();
    list["isComplex"] = m_gen->isComplexTypeEnabled();
    return list;
}

//-------------------------------------------------------------------------------------------------
void NoiseGeneratorCtrl::setParam(QVariantMap param)
{
    m_gen->setExpectation(param["expectation"].toDouble());
    m_gen->setDeviation(param["deviation"].toDouble());
    m_gen->setGeneratePeriod(param["genPeriod"].toInt());
    m_gen->setSampleRate(param["sampleRate"].toInt());
    m_gen->setComplexTypeEnabled(param["isComplex"].toBool());
    refreshUi();
}

//-------------------------------------------------------------------------------------------------
void NoiseGeneratorCtrl::refreshUi()
{
    ui->expectansionValueBox->setValue(m_gen->expectation());
    ui->deviationValueBox->setValue(m_gen->deviation());
    ui->periodValueBox->setValue(m_gen->generatePeriod());
    ui->sampleRateValueBox->setValue(m_gen->sampleRate());
    ui->complexSignalBox->setChecked(m_gen->isComplexTypeEnabled());
}

//-------------------------------------------------------------------------------------------------
void NoiseGeneratorCtrl::onChangeExpectationBox(double value)
{
    m_gen->setExpectation(value);
}

//-------------------------------------------------------------------------------------------------
void NoiseGeneratorCtrl::onChangeDeviationBox(double value)
{
    m_gen->setDeviation(value);
}

//-------------------------------------------------------------------------------------------------
void NoiseGeneratorCtrl::onChangeGenPeriodBox(int msec)
{
    m_gen->setGeneratePeriod(msec);
}

//-------------------------------------------------------------------------------------------------
void NoiseGeneratorCtrl::onChangeSampleRateBox(int hz)
{
    m_gen->setSampleRate(hz);
}

//-------------------------------------------------------------------------------------------------
void NoiseGeneratorCtrl::onComplexTypeBoxChecked(bool checked)
{
    m_gen->setComplexTypeEnabled(checked);
}

//-------------------------------------------------------------------------------------------------
void NoiseGeneratorCtrl::onDeviceStarted()
{
    ui->expectansionValueBox->setEnabled(false);
    ui->deviationValueBox->setEnabled(false);
    ui->periodValueBox->setEnabled(false);
    ui->sampleRateValueBox->setEnabled(false);
}

//-------------------------------------------------------------------------------------------------
void NoiseGeneratorCtrl::onDeviceStopped()
{
    ui->expectansionValueBox->setEnabled(true);
    ui->deviationValueBox->setEnabled(true);
    ui->periodValueBox->setEnabled(true);
    ui->sampleRateValueBox->setEnabled(true);
}
