#ifndef PLOTPEAKS_H
#define PLOTPEAKS_H

#include <QObject>
#include <QVector>
#include <QPointF>

class QwtPlot;
class QwtPlotCurve;

namespace Spectrum {

/*!
 * \brief Класс отрисовки отметок для пиковых гармоник
 */
class PeakPainter : public QObject
{
    Q_OBJECT

public:
    PeakPainter();

public slots:
    void replot(QVector <QPointF> peaks);
    void attach(QwtPlot* plot);
    void detach();

private:
    QwtPlot* m_plot;
    QwtPlotCurve* m_curve;
};

} ///< Spectrum

#endif // PLOTPEAKS_H
