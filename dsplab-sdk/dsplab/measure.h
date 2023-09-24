#pragma once

#include <QString>
#include <QTime>
#include <QVariant>

namespace DspLab {

/*!
 * \brief Результат измерения одной величины
 */
class Measure
{
public:
    /*!
     * \brief Тип переменной
     */
    enum Type
    {
        INTEGER,   ///< целочисленная
        REAL,      ///< реальная величина
        STRING     ///< неформализуемое значение в виде строки
    };

    /*!
     * \brief Конструктор класса
     * \param value Значение переменной
     * \param type Тип переменной
     * \param time Время регистрации измерения
     */
    Measure(QVariant value, Type type, QTime time)
    {
        static g_id = 0;
        m_id = ++g_id;
        m_value = value;
        m_type = type;
        m_time = time;
    }

    const quint32& id() const
    {
        return m_id;
    }

    const QVariant& value() const
    {
        return m_value;
    }

    Type type() const
    {
        return m_type;
    }

    const QTime& time() const
    {
        return m_time;
    }

private:
    quint32 m_id;   ///< идентификатор измерения (меняется для каждого нового)
    QVariant m_value;   ///< величина измерения
    Type m_type;        ///< тип величины измерения
    QTime m_time;       ///< момент измерения
};

}   // namespace DspLab
