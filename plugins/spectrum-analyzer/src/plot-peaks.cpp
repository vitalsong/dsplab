#include "plot-peaks.h"

#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include <qwt_symbol.h>

#define SYMB_SIZE (8)

namespace Spectrum {

//-------------------------------------------------------------------------------------------------
PeakPainter::PeakPainter()
{
    m_plot = nullptr;
    m_curve = new QwtPlotCurve();
    QwtSymbol* symbol = new QwtSymbol(QwtSymbol::DTriangle,
                                      QBrush(Qt::yellow),
                                      QPen(Qt::white),
                                      QSize(SYMB_SIZE, SYMB_SIZE));

    m_curve->setSymbol(symbol);
    m_curve->setStyle(QwtPlotCurve::NoCurve);
}

//-------------------------------------------------------------------------------------------------
void PeakPainter::replot(QVector<QPointF> peaks)
{
    //подтягиваем вверх
    for (QPointF& p : peaks) {
        p += QPointF(0, 2);
    }

    m_curve->setSamples(peaks);
}

//-------------------------------------------------------------------------------------------------
void PeakPainter::attach(QwtPlot *plot)
{
    m_plot = plot;
    m_curve->attach(plot);
}

//-------------------------------------------------------------------------------------------------
void PeakPainter::detach()
{
    m_curve->detach();
}

} ///< Spectrum
