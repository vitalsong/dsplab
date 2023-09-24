#pragma once

#include <QDialog>

#include <dsplab/controller.h>

namespace Ui {
class MainWindow;
}

namespace DspLab {

class SpectrumAnalyzer;
class SpectrumAnalyzerCtrl : public QDialog, public Controller
{
    Q_OBJECT

public:
    SpectrumAnalyzerCtrl();
    virtual ~SpectrumAnalyzerCtrl();
    QWidget* window();

private:
    Ui::MainWindow* ui;
    SpectrumAnalyzer* m_analyzer;
};

} /// DspLab
