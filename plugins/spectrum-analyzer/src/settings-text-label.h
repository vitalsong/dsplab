#ifndef SETTINGSTEXTLABEL_H
#define SETTINGSTEXTLABEL_H

#include <abstract-text-label.h>
#include <spectrum-common.h>

namespace Spectrum {

class SettingsTextLabel : public AbstractTextLabel
{
public:
    SettingsTextLabel(QWidget* parent, Position pos = BottomLeft);

    //состояние для отображения
    void setState(State state);
    State getState() const;

private:
    State m_state;
};

} ///< Spectrum

#endif // SETTINGSTEXTLABEL_H
