#include <spectrum-analyzer-ctrl.h>
#include <spectrum-analyzer.h>
#include "ui_main-window.h"

using namespace DspLab;

//-------------------------------------------------------------------------------------------
SpectrumAnalyzerCtrl::SpectrumAnalyzerCtrl() :
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QDialog::setWindowTitle("Spectrum Analyzer");

    //анализатор
    m_analyzer = new SpectrumAnalyzer();
    Controller::setDeviceModel(m_analyzer);

    //добаляем график
    ui->mainLayout->addWidget(m_analyzer->getMainPlot());

    //старт при создании
    m_analyzer->start();
}

//-------------------------------------------------------------------------------------------
SpectrumAnalyzerCtrl::~SpectrumAnalyzerCtrl()
{
    delete m_analyzer;
    delete ui;
}

//-------------------------------------------------------------------------------------------
QWidget *SpectrumAnalyzerCtrl::window()
{
    return this;
}
