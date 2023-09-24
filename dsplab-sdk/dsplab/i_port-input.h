#pragma once

#include <vector>

#include <dsplab/frame.h>

namespace DspLab {

class IDevice;
class IPortOutput;

/*!
 * \brief Интерфейс входного порта устройства
 */
class IPortInput
{
public:
    virtual ~IPortInput()
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
     * \brief Записать фрейм в порт
     * \param frame Ссылка на фрейм для записи
     */
    virtual void write(const Frame& frame) = 0;

    /*!
     * \brief Коммутация с выходным портом
     * \param port Указатель на выходной порт
     * \return Успех операции
     */
    virtual bool attach(IPortOutput* port) = 0;

    /*!
     * \brief Разрыв связи с выходным портом
     * \param port Указатель на выходной порт
     * \return Успех операции
     */
    virtual bool detach(IPortOutput* port) = 0;

    /*!
     * \brief Разрыв связи со всеми подключенными выходными портами
     * По факту к входному порту может быть подключен только один выходной
     */
    virtual void detachAll() = 0;

    /*!
     * \brief Получить список подключений
     * \return Список подключений
     */
    virtual std::vector<IPortOutput*> connections() = 0;
};

}   // namespace DspLab
