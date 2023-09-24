#include "sinus-generator-ctrl.h"
#include "sinus-generator.h"
#include "ui_main-window.h"

#include <QtCore>

using namespace DspLab;

//-------------------------------------------------------------------------------------------------
SinusGeneratorCtrl::SinusGeneratorCtrl()
  : ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_singen = new SinusGenerator();
    this->setWindowTitle("Sinus Generator");
    this->setDeviceModel(m_singen);

    //реакция на изменение параметров
    connect(ui->sampleRateBox, SIGNAL(editingFinished()), this, SLOT(onSampleRateEditingFinished()));
    connect(ui->toneFreqBox, SIGNAL(editingFinished()), this, SLOT(onFrequensyEditingFinished()));
    connect(ui->toneAmpBox, SIGNAL(editingFinished()), this, SLOT(onAmplitudeEditingFinished()));
    connect(ui->fullScaleBox, SIGNAL(editingFinished()), this, SLOT(onFullScaleEditingFinished()));
    connect(ui->genPeriodBox, SIGNAL(editingFinished()), this, SLOT(onGeneratePeriodEditingFinished()));
}

//-------------------------------------------------------------------------------------------------
SinusGeneratorCtrl::~SinusGeneratorCtrl()
{
    delete m_singen;
    delete ui;
}

//-------------------------------------------------------------------------------------------------
QVariantMap SinusGeneratorCtrl::paramList() const
{
    QVariantMap list;
    list["sampleRate"] = m_singen->getSampleRateHz();
    list["toneFreqHz"] = m_singen->getFrequensyHz();
    list["toneAmpDb"] = m_singen->getAmplitudeDB();
    list["fullScale"] = m_singen->getFullScale();
    list["generatePeriod"] = m_singen->getGeneratePeriod();
    return list;
}

//-------------------------------------------------------------------------------------------------
void SinusGeneratorCtrl::setParam(QVariantMap list)
{
    m_singen->setSampleRateHz(list["sampleRate"].toInt());
    m_singen->setFrequensyHz(list["toneFreqHz"].toInt());
    m_singen->setAmplitudeDB(list["toneAmpDb"].toDouble());
    m_singen->setFullScale(list["fullScale"].toDouble());
    m_singen->setGeneratePeriod(list["generatePeriod"].toInt());
    onRefreshParamUi();
}

//-------------------------------------------------------------------------------------------------
QWidget* SinusGeneratorCtrl::window()
{
    return this;
}

//-------------------------------------------------------------------------------------------------
void SinusGeneratorCtrl::onSampleRateEditingFinished()
{
    int value = ui->sampleRateBox->value();
    m_singen->setSampleRateHz(value);
}

//-------------------------------------------------------------------------------------------------
void SinusGeneratorCtrl::onFrequensyEditingFinished()
{
    int value = ui->toneFreqBox->value();
    m_singen->setFrequensyHz(value);
}

//-------------------------------------------------------------------------------------------------
void SinusGeneratorCtrl::onAmplitudeEditingFinished()
{
    double value = ui->toneAmpBox->value();
    m_singen->setAmplitudeDB(value);
}

//-------------------------------------------------------------------------------------------------
void SinusGeneratorCtrl::onFullScaleEditingFinished()
{
    double value = ui->fullScaleBox->value();
    m_singen->setFullScale(value);
}

//-------------------------------------------------------------------------------------------------
void SinusGeneratorCtrl::onGeneratePeriodEditingFinished()
{
    int value = ui->genPeriodBox->value();
    m_singen->setGeneratePeriod(value);
}

//-------------------------------------------------------------------------------------------------
void SinusGeneratorCtrl::onRefreshParamUi()
{
    ui->sampleRateBox->setValue(m_singen->getSampleRateHz());
    ui->toneFreqBox->setValue(m_singen->getFrequensyHz());
    ui->toneAmpBox->setValue(m_singen->getAmplitudeDB());
    ui->fullScaleBox->setValue(m_singen->getFullScale());
    ui->genPeriodBox->setValue(m_singen->getGeneratePeriod());
}

//------------------------------------------------------------------------------------------
void SinusGeneratorCtrl::onEnableUi()
{
    ui->sampleRateBox->setEnabled(true);
    ui->fullScaleBox->setEnabled(true);
    ui->genPeriodBox->setEnabled(true);
}

//------------------------------------------------------------------------------------------
void SinusGeneratorCtrl::onDisableUi()
{
    ui->sampleRateBox->setEnabled(false);
    ui->fullScaleBox->setEnabled(false);
    ui->genPeriodBox->setEnabled(false);
}
