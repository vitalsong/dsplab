#ifndef SPECTRUMPLOTMENU_H
#define SPECTRUMPLOTMENU_H

#include <QObject>

#include "spectrum-common.h"

class QMenu;

namespace Spectrum {

/*!
 * \brief Класс контексного меню анализатора спектра
 * \details Вызывается при нажатии правой кнопки в области родителя
 */
class PlotMenu : public QObject
{
    Q_OBJECT

public:
    PlotMenu(QWidget* parent);
    virtual bool eventFilter(QObject *watched, QEvent *event);

    void setState(State state);
    State getState() const;

signals:
    void fftSizeChanged(int);
    void fftWindowChanged(Spectrum::FftWindow);
    void accumModeChanged(Spectrum::AccumulationMode);
    void accumFactorChanged(int);
    void addNoiseEnableChanged(bool);

private:
    void _exec(QPoint pos);
    void _addFftSizeAction(QMenu &menu, int size);

private:
    State m_state;
};

} ///< Spectrum

#endif // SPECTRUMPLOTMENU_H
