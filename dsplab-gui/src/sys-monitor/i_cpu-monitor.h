#pragma once

class ICpuMonitor
{
public:
    virtual ~ICpuMonitor()
    {}

    /*!
     * \brief Текущая нагрузка процесса
     * \return Возвращает нагрузку текущего процесса в процентах
     */
    virtual int current() const = 0;
};
