#pragma once

#include <dsplab/i_device.h>
#include <dsplab/display-value.h>

#include <QVariantMap>

#include <vector>

class QWidget;

namespace DspLab {

/*!
 * \brief Интерфейс контроллера устройств с GUI
 */
class IController
{
public:
    virtual ~IController()
    {}

    /*!
     * \brief Получить указатель на модель устройства
     * \return
     */
    virtual IDevice* device() = 0;

    /*!
     * \brief Получить указатель на окно пользовательского интерфейса
     * \return
     */
    virtual QWidget* window() = 0;

    /*!
     * \brief Получить список текущих настроек
     *
     * \details Функция должна возвращать только те настройки,
     * которые сможет (и считает нужным) декодировать в функции setParam.
     * Объекты парамтров необходимы, чтобы позволить сохранить их в отдельном конфигурационном файле
     * и восстановить рабочую сессию без повторной настройки устройства. Также, может быть полезным
     * для режима без графической оболочки.
     *
     * \return Список с параметрами
     */
    virtual QVariantMap paramList() const = 0;

    /*!
     * \brief Настройка параметров модуля
     * \param param Объект параметров
     */
    virtual void setParam(QVariantMap param) = 0;

    /*!
     * \brief Количество значений для отображения
     * \return Кол-во значений
     */
    virtual int valuesCount() const = 0;

    /*!
     * \brief Список значений для отображения
     * \return Список значений
     */
    virtual std::vector<DisplayValue> valuesList() const = 0;
};

}   // namespace DspLab
