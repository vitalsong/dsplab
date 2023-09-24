#pragma once

#include <qwt_plot.h>

class QwtPlotGrid;
class QwtPlotCurve;
class QwtPlotPicker;
class QwtTextLabel;
class QVBoxLayout;

/*!
 * \brief Класс для рисования спектра
 */
class SpectrumPlot : public QwtPlot
{
    Q_OBJECT

public:
    SpectrumPlot(QWidget* parent = 0);
    virtual ~SpectrumPlot();

public slots:
    void drawSpectrum(QVector<QPointF> spectrum);
    void setAxisScaleX(double min, double max, double step = 0);
    void setAxisScaleY(double min, double max, double step = 0);

private slots:
    //прореживание точек перед отрисовкой
    void _downsample(QVector<QPointF>& points);

private:
    QwtPlot* m_plot;
    QwtPlotGrid* m_grid;
    QwtPlotCurve* m_curve;
    QwtPlotPicker* m_picker;
};
