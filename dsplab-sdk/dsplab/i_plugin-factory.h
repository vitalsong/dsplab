#pragma once

#include <dsplab/i_controller.h>

#include <memory>

namespace DspLab {

/*!
 * \brief Интерфейс фабрики плагинов
 * \details Должен быть доступен в библиотеке плагинов (и скорее всего реализован как синглтон)
 */
class IPluginFactory
{
public:
    /*!
     * \brief Конструктор объектов контроллеров
     * \return Умный указатель на объект контроллера
     */
    virtual std::shared_ptr<IController> create() = 0;

protected:
    virtual ~IPluginFactory()
    {}
};

}   // namespace DspLab
