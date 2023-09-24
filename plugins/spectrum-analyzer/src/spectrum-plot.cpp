#include "spectrum-plot.h"

#include <qwt_plot_curve.h>
#include <qwt_plot_grid.h>
#include <qwt_plot_picker.h>
#include <qwt_picker_machine.h>
#include <qwt_text_label.h>
#include <QPen>

//-------------------------------------------------------------------------------------------------
SpectrumPlot::SpectrumPlot(QWidget* parent) : QwtPlot(parent)
{
    //создаем основные графические элементы
    m_curve = new QwtPlotCurve();
    m_grid = new QwtPlotGrid();
    m_picker = new QwtPlotPicker(QwtPlot::xBottom, QwtPlot::yLeft, QwtPlotPicker::CrossRubberBand,
                                 QwtPicker::ActiveOnly, QwtPlot::canvas());

    //настройка основного поля отображения
    QwtPlot::setCanvasBackground(Qt::black);
    QwtPlot::enableAxis(QwtPlot::xBottom, false);
    QwtPlot::enableAxis(QwtPlot::yLeft, true);

    //настройка кривой
    m_curve->setRenderHint(QwtPlotItem::RenderAntialiased, true);
    m_curve->setPen(QColor(48, 192, 255, 255), 1);
    m_curve->setBrush(QColor(0, 48, 110, 255));
    m_curve->setBaseline(-200);
    m_curve->attach(this);

    //настройка сетки
    m_grid->setMajorPen(QPen(Qt::lightGray, 0.5));
    m_grid->attach(this);

    //настройка указателя мыши
    m_picker->setRubberBandPen(QColor(Qt::blue));
    m_picker->setTrackerPen(QColor(Qt::white));
    m_picker->setStateMachine(new QwtPickerDragPointMachine());
    m_picker->setEnabled(true);
}

//-------------------------------------------------------------------------------------------------
SpectrumPlot::~SpectrumPlot()
{
    delete m_curve;
    delete m_grid;
    delete m_picker;
}

//-------------------------------------------------------------------------------------------------
void SpectrumPlot::drawSpectrum(QVector<QPointF> spectrum)
{
    _downsample(spectrum);
    m_curve->setSamples(spectrum);
    QwtPlot::replot();
}

//-------------------------------------------------------------------------------------------------
void SpectrumPlot::setAxisScaleX(double min, double max, double step)
{
    QwtPlot::setAxisScale(QwtPlot::xBottom, min, max, step);
}

//-------------------------------------------------------------------------------------------------
void SpectrumPlot::setAxisScaleY(double min, double max, double step)
{
    QwtPlot::setAxisScale(QwtPlot::yLeft, min, max, step);
}

//-------------------------------------------------------------------------------------------------
void SpectrumPlot::_downsample(QVector<QPointF> &points)
{
    //на практике отрисовка более 1024 точек
    //в маленьком окне на экране с шириной 1080 пикселей не требуется
    const int MAX_COUNT = 1024;

    //выбор оптимальной степени децимации
    const int N = points.size();
    int d = 1;
    while (N / d > MAX_COUNT) {
        ++d;
    }

    //если децимация не требуется
    if (d == 1) {
        return;
    }

    //децимация с выбором максимума
    int M = N / d;
    QVector<QPointF> r;
    for (int i=0; i < M; ++i)
    {
        auto p1 = points.begin() + (i * d);
        auto p2 = p1 + d;
        QPointF max = *std::max_element(p1, p2, [](QPointF lhs, QPointF rhs){return lhs.y() < rhs.y();});
        r.push_back(max);
    }

    points = r;
}
