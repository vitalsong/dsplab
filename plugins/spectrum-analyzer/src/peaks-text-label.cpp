#include "peaks-text-label.h"

#define MIN_DB_VALUE (-200)

namespace Spectrum {

//-------------------------------------------------------------------------------------------------
PeaksTextLabel::PeaksTextLabel(QWidget *parent, Position pos) :
    AbstractTextLabel(parent, pos)
{
    //nothing to do
}

//-------------------------------------------------------------------------------------------------
PeaksTextLabel::~PeaksTextLabel()
{
    //nothing to do
}

//-------------------------------------------------------------------------------------------------
void PeaksTextLabel::setPeaks(QVector<QPointF> peaks)
{
    QString text;
    for (int i=0; i < peaks.size(); ++i)
    {
        if (peaks[i].y() > MIN_DB_VALUE)
        {
            text += QString("f") + QString::number(i) + " " +
                    QString::number(peaks[i].y(), 'f', 2) + " dB : " +
                    QString::number(peaks[i].x(), 'f', 2) + " Hz\n";
        }
        else {
            text += QString("f") + QString::number(i) + " - \n";
        }
    }

    //удаляем последний символ переноса
    text.remove(text.size()-1, 1);

    AbstractTextLabel::setText(text);
    AbstractTextLabel::repaint();
}

} ///< Spectrum
