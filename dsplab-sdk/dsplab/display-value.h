#pragma once

#include <QString>

namespace DspLab {

class DisplayValue
{
public:
    enum VisibleMode
    {
        VALUE,
        VALUE_UNIT,
        NAME_VALUE,
        NAME_VALUE_UNIT,
    };

    DisplayValue();
    DisplayValue(QString name, QString value, VisibleMode visible = NAME_VALUE_UNIT, QString unit = {});
    DisplayValue(QString name, int value, VisibleMode visible = NAME_VALUE_UNIT, QString unit = {});
    DisplayValue(QString name, double value, int prec, VisibleMode visible = NAME_VALUE_UNIT, QString unit = {});

    const QString& name() const;
    const QString& value() const;
    const QString& unit() const;
    VisibleMode visible() const;
    QString label() const;

private:
    QString m_name;
    QString m_value;
    QString m_unit;
    VisibleMode m_visible;
};

}   // namespace DspLab
