#pragma once

namespace DspLab {

/*!
 * \brief Основные форматы
 */
enum BaseType
{
    Real,      ///< реальная последовательность
    Complex,   ///< комплексная последовательность
    Special   ///< специальный формат (может быть расширен пользователем)
};

/*!
 * \brief Идентификаторы для расширения базового спец.формата
 * \details Список будет расширяться, поэтому желательно зарезервировать диапазон под добавление новых значений
 */
enum SpecialExtension
{
    Raw,       ///< байтовая последовательность
    Ref,       ///< формат для синхронизации (в разработке)
    RealInt,   ///< целочисленная последовательность (в разработке)
    CmplxInt   ///< комплексная последовательность (в разработке)
};

/*!
 * \brief Класс формата фрейма
 */
class Format
{
public:
    Format(BaseType type = Real, int special = Raw)
    {
        m_type = type;
        m_special = special;
    }

    BaseType base() const
    {
        return m_type;
    }

    int special() const
    {
        return m_special;
    }

    bool operator==(const Format& rhs) const
    {
        return (m_type == rhs.m_type) && (m_special == rhs.m_special);
    }

    bool operator==(BaseType type) const
    {
        return (this->m_type == type);
    }

    bool operator==(int specialId) const
    {
        return this->operator==(Format(Special, specialId));
    }

    bool operator!=(const Format& rhs) const
    {
        return !(this->operator==(rhs));
    }

    bool operator!=(BaseType type) const
    {
        return !(this->operator==(type));
    }

    bool operator!=(int specialId) const
    {
        return !(this->operator==(specialId));
    }

private:
    BaseType m_type;
    int m_special;
};

}   // namespace DspLab
