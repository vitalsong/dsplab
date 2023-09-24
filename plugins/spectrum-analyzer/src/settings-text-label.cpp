#include <settings-text-label.h>
#include <QEvent>

namespace Spectrum {

//-------------------------------------------------------------------------------------------------
SettingsTextLabel::SettingsTextLabel(QWidget *parent, Position pos) :
    AbstractTextLabel(parent, pos)
{
    m_state.fft_size = 512;
    m_state.accum_mode = NotAccumulation;
    m_state.accum_size = 0;
    m_state.add_noise = false;
    m_state.wind_mode = Hann;
}

//-------------------------------------------------------------------------------------------------
static QString _formText(State state)
{
    QString text;

    //основание БПФ
    text = QString("fft size: ") + QString::number(state.fft_size) + QString("\n");

    //окно бпф
    QString win;
    switch (state.wind_mode)
    {
    case Spectrum::Rectangle: win = "rectangle";
        break;
    case Spectrum::Sinus: win = "sinus";
        break;
    case Spectrum::Hann: win = "hann";
        break;
    case Spectrum::Hamming: win = "hamming";
        break;
    case Spectrum::Blackman: win = "blackman";
        break;
    case Spectrum::Gauss: win = "gauss";
        break;
    }

    text += QString("window: ") + win + QString("\n");

    //режим накопления
    QString accum;
    switch (state.accum_mode)
    {
    case Spectrum::NotAccumulation: accum = "normal";
        break;
    case Spectrum::AccumulationMax: accum = "max";
        break;
    case Spectrum::AccumulationMin: accum = "min";
        break;
    case Spectrum::AccumulationMean: accum = "mean";
        break;
    }

    text += QString("mode: ") + accum + QString("\n");

    //степень накопления
    switch (state.accum_mode)
    {
    case Spectrum::NotAccumulation:
    case Spectrum::AccumulationMax:
    case Spectrum::AccumulationMin:
        text += QString("mean size: -");
        break;
    case Spectrum::AccumulationMean:
        text += QString("mean size: ") + QString::number(state.accum_size);
        break;
    }

    return text;
}

//-------------------------------------------------------------------------------------------------
void SettingsTextLabel::setState(State state)
{
    m_state = state;
    QString text = _formText(m_state);
    AbstractTextLabel::setText(text);
    AbstractTextLabel::repaint();
}

//-------------------------------------------------------------------------------------------------
State SettingsTextLabel::getState() const
{
    return m_state;
}

} ///< Spectrum
