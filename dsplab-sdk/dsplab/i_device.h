#pragma once

#include <QObject>

#include <dsplab/i_port-input.h>
#include <dsplab/i_port-output.h>

namespace DspLab {

/*!
 * \brief Базовый интерфейс устройств динамической обработки
 */
class IDevice : public QObject
{
    Q_OBJECT

public:
    virtual ~IDevice()
    {}

    /*!
     * \brief Получить указатель на входной порт под номером
     * \param num Номер порта
     * \return
     */
    virtual IPortInput* input(int num) const = 0;

    /*!
     * \brief Получить указатель на выходной порт под номером
     * \param num Номер порта
     * \return
     */
    virtual IPortOutput* output(int num) const = 0;

    /*!
     * \brief Получить количество входов устройства.
     * \return Количество входов
     */
    virtual int num_inputs() const = 0;

    /*!
     * \brief Получить количество выходов устройства.
     * \return Количество выходов
     */
    virtual int num_outputs() const = 0;

    /*!
     * \brief Включить устройство.
     */
    virtual void start() = 0;

    /*!
     * \brief Отключить устройство.
     */
    virtual void stop() = 0;

    /*!
     * \brief Получить текущее состояние.
     * \return Текущее состояние
     */
    virtual bool started() const = 0;

    /*!
     * \brief Получить текущее состояние ожидания
     * \return TRUE - если перешел в режим ожидания, FALSE - если вернулся в рабочий режим
     */
    virtual bool standby() const = 0;

    /*!
     * \brief Активация устройства.
     *
     * Функция инициативного вызова. На каждый вызов activate() необходимо вызывать
     * activate() у всех устройств, подключенных к выходу, независимо от наличия обработанных порций данных.
     */
    virtual void activate() = 0;

signals:

    /*!
     * \brief Уведомление о запуске устройства
     */
    void deviceStarted();

    /*!
     * \brief Уведомление об остановке устройства
     */
    void deviceStopped();

    /*!
     * \brief Уведомление о переходе в режим ожидания
     * \param enter TRUE - если перешел в режим ожидания, FALSE - если вернулся в рабочий режим
     */
    void deviceStandby(bool enter);
};

}   // namespace DspLab
