#pragma once

#include <vector>

#include <dsplab/frame.h>

namespace DspLab {

class IDevice;
class IPortInput;

/*!
 * \brief Интерфейс выходного порта устройства
 */
class IPortOutput
{
public:
    virtual ~IPortOutput()
    {}

    /*!
     * \brief Получить указатель на устройство-родителя
     * \return Указатель на устройство-родителя
     */
    virtual IDevice* parent() const = 0;

    /*!
     * \brief Получить список доступных форматов данного порта
     * \details Необходимо для фильтрации входящих фреймов
     * \return Вектор с доступными форматами
     */
    virtual std::vector<Format> avaliableFormats() const = 0;

    /*!
     * \brief Подключение к входному порту
     * \param port Указатель на входной порт
     * \return Успех операции
     */
    virtual bool attach(IPortInput* port) = 0;

    /*!
     * \brief Отключение от входного порта
     * \param port Указатель на входной порт
     * \return Успех операции
     */
    virtual bool detach(IPortInput* port) = 0;

    /*!
     * \brief Разрыв всех соединений
     */
    virtual void detachAll() = 0;

    /*!
     * \brief Получить список подключений
     * \return Список подключений
     */
    virtual std::vector<IPortInput*> connections() = 0;
};

}   // namespace DspLab
