#ifndef PEAKSTEXTLABEL_H
#define PEAKSTEXTLABEL_H

#include <QVector>
#include <QPointF>

#include <abstract-text-label.h>

namespace Spectrum {

class PeaksTextLabel : public AbstractTextLabel
{
    Q_OBJECT

public:
    PeaksTextLabel(QWidget* parent, Position pos = BottomRight);
    virtual ~PeaksTextLabel();

public slots:
    void setPeaks(QVector <QPointF> peaks);
};

} ///< Spectrum

#endif // PEAKSTEXTLABEL_H
