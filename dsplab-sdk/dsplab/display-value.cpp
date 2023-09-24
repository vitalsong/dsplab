#include "display-value.h"
#include <string>

using namespace DspLab;

//-------------------------------------------------------------------------------------------------
DisplayValue::DisplayValue()
{
    m_name = "";
    m_value = "";
    m_unit = "";
    m_visible = NAME_VALUE_UNIT;
}

//-------------------------------------------------------------------------------------------------
DisplayValue::DisplayValue(QString name, QString value, VisibleMode visible, QString unit)
{
    m_name = name;
    m_value = value;
    m_visible = visible;
    m_unit = unit;
}

//-------------------------------------------------------------------------------------------------
DisplayValue::DisplayValue(QString name, int value, VisibleMode visible, QString unit)
{
    m_name = name;
    m_value = QString::number(value);
    m_visible = visible;
}

//-------------------------------------------------------------------------------------------------
DisplayValue::DisplayValue(QString name, double value, int prec, VisibleMode visible, QString unit)
{
    m_name = name;
    m_value = QString::number(value, 10, prec);
    m_visible = visible;
    m_unit = unit;
}

//-------------------------------------------------------------------------------------------------
const QString& DisplayValue::name() const
{
    return m_name;
}

//-------------------------------------------------------------------------------------------------
const QString& DisplayValue::value() const
{
    return m_value;
}

//-------------------------------------------------------------------------------------------------
const QString& DisplayValue::unit() const
{
    return m_unit;
}

//-------------------------------------------------------------------------------------------------
DisplayValue::VisibleMode DisplayValue::visible() const
{
    return m_visible;
}

//-------------------------------------------------------------------------------------------------
QString DisplayValue::label() const
{
    QString label;

    switch (m_visible) {
    case VALUE:
        label = m_value;
        break;
    case VALUE_UNIT:
        label = m_value + " " + m_unit;
        break;
    case NAME_VALUE:
        label = m_name + ": " + m_value;
        break;
    case NAME_VALUE_UNIT:
        label = m_name + ": " + m_value + " " + m_unit;
        break;
    }

    return label;
}
