#pragma once

#include <dsplab/types.h>
#include <dsplab/format.h>
#include <dsplab/meta.h>

#include <vector>
#include <memory>

namespace DspLab {

/*!
 * \brief Фрейм с данными
 * \details Используется как базовая единица обмена данными между устройствами.
 * \todo 1) Добавить функцию swap для быстрого обмена данными
 * 2) Использовать общий (правильно выровненный!) буфер для всех форматов.
 * 3) Добавить копирующий rvalue конструктор (C++11)
 */
class Frame
{
public:
    class Impl;

    Frame();
    Frame(const Frame& frame);

    class RealData
    {
    public:
        RealData(const real_t* data, int size);
        RealData(const real_t* data, int size, DspMeta meta);
        const real_t* data() const;
        int size() const;
        DspMeta meta() const;

    private:
        const real_t* m_data;
        int m_size;
        DspMeta m_meta;
    };

    class ComplexData
    {
    public:
        ComplexData(const complex_t* data, int size);
        ComplexData(const complex_t* data, int size, DspMeta meta);
        const complex_t* data() const;
        int size() const;
        DspMeta meta() const;

    private:
        const complex_t* m_data;
        int m_size;
        DspMeta m_meta;
    };

    class SpecialData
    {
    public:
        SpecialData(const char* data, int size, int id);
        const char* data() const;
        int size() const;
        int id() const;

    private:
        const char* m_data;
        int m_size;
        int m_id;
    };

    Frame(RealData data);
    Frame(ComplexData data);
    Frame(SpecialData data);

    /*!
     * \brief Получить формат фрейма
     * \return Объект формата
     */
    Format format() const;

    RealData getReal() const;
    ComplexData getComplex() const;
    SpecialData getSpecial() const;

    void setReal(RealData pack);
    void setComplex(ComplexData pack);
    void setSpecial(SpecialData pack);

private:
    std::shared_ptr<Impl> m_impl;
};

}   // namespace DspLab
