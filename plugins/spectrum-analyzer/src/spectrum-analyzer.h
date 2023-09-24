#pragma once

#include <dsplab/device.h>

#include <QVector>
#include <QPointF>

#include <dsplib.h>

#include <spectrum-common.h>

class QWidget;
class SpectrumPlot;

namespace Spectrum
{
class Process;
class PlotMenu;
class SettingsTextLabel;
class PeaksTextLabel;
class PeakPainter;
}

namespace DspLab
{
class SpectrumAnalyzer : public Device
{
    Q_OBJECT

public:
    SpectrumAnalyzer();
    virtual ~SpectrumAnalyzer();
    void processing() final;
    QWidget* getMainPlot() const;

private:
    void _readReal();
    void _readComplex();
    void _checkFormat(Format format);
    void _checkMeta(DspMeta meta);
    void _rescaleAxisX();
    void _refreshState();
    void _configConnection();

private slots:
    void onSetFftSize(int nfft);
    void onSetFftWindow(Spectrum::FftWindow wind);
    void onSetAccumulationRate(int count);
    void onSetAccumulationMode(Spectrum::AccumulationMode mode);

private:
    SpectrumPlot* m_plot;          ///< дисплей для отображения
    Spectrum::Process* m_proc;
    Spectrum::PlotMenu* m_menu;
    Spectrum::SettingsTextLabel* m_info;
    Spectrum::PeaksTextLabel* m_peaks_info;
    Spectrum::PeakPainter* m_peaks_symb;

    Frame m_frame;                  ///< хранение входного фрейма
    Format m_format;                ///< текущий формат отображения
};

} /// DspLab
