#include <spectrum-analyzer.h>
#include <spectrum-plot.h>

#include <qmath.h>
#include <assert.h>

#include "plot-menu.h"
#include "settings-text-label.h"
#include "peaks-text-label.h"
#include "spectrum-process.h"
#include "plot-peaks.h"

using namespace DspLab;

//-------------------------------------------------------------------------------------------------
SpectrumAnalyzer::SpectrumAnalyzer()
  : Device(1, 0)
  , m_plot(nullptr)
  , m_format(Special, Raw)
{
    //вычислитель
    m_proc = new Spectrum::Process();
    qRegisterMetaType<QVector<QPointF>>("QVector<QPointF>");

    //граф.поле для рисования спектра
    m_plot = new SpectrumPlot();
    m_plot->setAxisScaleY(-100, 0, 12);
    _rescaleAxisX();

    //привязка контекстного меню
    m_menu = new Spectrum::PlotMenu(m_plot);

    //привязка текста с отображением информации
    m_info = new Spectrum::SettingsTextLabel(m_plot->canvas());

    //привязка информации о пиковых частотах
    m_peaks_info = new Spectrum::PeaksTextLabel(m_plot->canvas());

    //отрисовка отметок над пиками
    m_peaks_symb = new Spectrum::PeakPainter();
    m_peaks_symb->attach(m_plot);

    //настройка входного порта
    Device::setAvaliableInputFormats({Real, Complex});

    //обновление состояние элементов управления и отображения
    _refreshState();

    //концигурация соединений
    _configConnection();
}

//-------------------------------------------------------------------------------------------------
void SpectrumAnalyzer::_configConnection()
{
    connect(m_proc, SIGNAL(spectrumReady(QVector<QPointF>)), m_plot, SLOT(drawSpectrum(QVector<QPointF>)));
    connect(m_menu, SIGNAL(fftSizeChanged(int)), this, SLOT(onSetFftSize(int)));
    connect(m_menu, SIGNAL(fftWindowChanged(Spectrum::FftWindow)), this, SLOT(onSetFftWindow(Spectrum::FftWindow)));
    connect(m_menu, SIGNAL(accumModeChanged(Spectrum::AccumulationMode)), this,
            SLOT(onSetAccumulationMode(Spectrum::AccumulationMode)));
    connect(m_menu, SIGNAL(accumFactorChanged(int)), this, SLOT(onSetAccumulationRate(int)));
    connect(m_proc, SIGNAL(peaksReady(QVector<QPointF>)), m_peaks_info, SLOT(setPeaks(QVector<QPointF>)));
    connect(m_proc, SIGNAL(peaksReady(QVector<QPointF>)), m_peaks_symb, SLOT(replot(QVector<QPointF>)));
}

//-------------------------------------------------------------------------------------------------
SpectrumAnalyzer::~SpectrumAnalyzer()
{
    SpectrumAnalyzer::stop();
    delete m_proc;
    delete m_plot;
}

//-------------------------------------------------------------------------------------------------
void SpectrumAnalyzer::processing()
{
    //проверяем входные буферы
    while (Device::ioSize() > 0) {
        //чтение фрейма
        Device::ioRead(m_frame);

        //проверка формата фрейма (на изменение)
        _checkFormat(m_frame.format());

        //обработка в зависимости от формата
        if (m_format == Real) {
            _readReal();
        } else if (m_format == Complex) {
            _readComplex();
        }
    }
}

//-------------------------------------------------------------------------------------------------
QWidget* SpectrumAnalyzer::getMainPlot() const
{
    return m_plot;
}

//-------------------------------------------------------------------------------------------------
void SpectrumAnalyzer::_readReal()
{
    auto package = m_frame.getReal();
    const real_t* data = package.data();
    int size = package.size();

    //если метаинформация о сигнале изменилась
    _checkMeta(package.meta());

    //отправляем в вычислитель
    m_proc->write(dsplib::arr_real(data, size));
}

//-------------------------------------------------------------------------------------------------
void SpectrumAnalyzer::_readComplex()
{
    auto package = m_frame.getComplex();
    const dsplib::cmplx_t* data = (const dsplib::cmplx_t*)package.data();
    int size = package.size();

    //если метаинформация о сигнале изменилась
    _checkMeta(package.meta());

    //отправляем в вычислитель
    m_proc->write(dsplib::arr_cmplx(data, size));
}

//-------------------------------------------------------------------------------------------------
void SpectrumAnalyzer::_checkFormat(Format format)
{
    if (format != m_format) {
        m_format = format;
        _rescaleAxisX();
    }
}

//-------------------------------------------------------------------------------------------------
void SpectrumAnalyzer::_checkMeta(DspMeta meta)
{
    //если метаинформация о сигнале изменилась
    int old_fs = m_proc->getSampleRate();
    int old_scale = round(m_proc->getFullScale());
    int new_fs = round(meta.sampleRate);
    int new_scale = round(meta.scalePerParrots);

    if (old_fs != new_fs) {
        m_proc->setSampleRate(new_fs);
        _rescaleAxisX();
    }

    if (old_scale != new_scale) {
        m_proc->setFullScale(new_scale);
    }
}

//-------------------------------------------------------------------------------------------------
void SpectrumAnalyzer::_rescaleAxisX()
{
    int fs = m_proc->getSampleRate();
    if (m_format == Real) {
        m_plot->setAxisScaleX(0, fs / 2, fs / 20);
    } else if (m_format == Complex) {
        m_plot->setAxisScaleX(-fs / 2, fs / 2, fs / 20);
    }
}

//-------------------------------------------------------------------------------------------------
void SpectrumAnalyzer::_refreshState()
{
    Spectrum::State state;
    state.fft_size = m_proc->getFftSize();
    state.wind_mode = m_proc->getFftWindow();
    state.accum_mode = m_proc->getAccumulationMode();
    state.accum_size = m_proc->getAccumulationRate();
    state.add_noise = false;

    m_menu->setState(state);
    m_info->setState(state);
}

//-------------------------------------------------------------------------------------------------
void SpectrumAnalyzer::onSetFftSize(int nfft)
{
    m_proc->setFftSize(nfft);
    _refreshState();
}

//-------------------------------------------------------------------------------------------------
void SpectrumAnalyzer::onSetFftWindow(Spectrum::FftWindow wind)
{
    m_proc->setFftWindow(wind);
    _refreshState();
}

//-------------------------------------------------------------------------------------------------
void SpectrumAnalyzer::onSetAccumulationRate(int count)
{
    m_proc->setAccumulationRate(count);
    _refreshState();
}

//-------------------------------------------------------------------------------------------------
void SpectrumAnalyzer::onSetAccumulationMode(Spectrum::AccumulationMode mode)
{
    m_proc->setAccumulationMode(mode);
    _refreshState();
}
