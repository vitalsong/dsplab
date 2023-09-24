#pragma once

#include <dsplab/i_device.h>

#include <mutex>
#include <vector>
#include <atomic>
#include <chrono>
#include <condition_variable>

class QTimer;

namespace DspLab {

class Frame;
class Timer;
class PortInput;
class PortOutput;

/*!
 * \brief Абстрактный класс устройства с реализацией основного функционала
 * \warning Перед удалением устройства необходимо использовать остановку
 */
class Device : public IDevice
{
    Q_OBJECT

public:
    Device(int nin = 1, int nout = 1);
    virtual ~Device() override;

    IPortInput* input(int num) const override;
    IPortOutput* output(int num) const override;

    int num_inputs() const final;
    int num_outputs() const final;

    void start() override;
    void stop() override;
    bool started() const final;
    bool standby() const final;
    void activate() final;

    /*!
     * \brief Основная функция обработки.
     * \details У каждого устройства вызывается в отдельном потоке. Реализуется пользователем.
     */
    virtual void processing() = 0;

protected:
    /*!
     * \brief Отправка фрейма по данному выходному порту
     * \param data Ссылка на фрейм
     * \param numSelfOut Номер выхода
     */
    void ioWrite(const Frame& frame, int numSelfOut = 0);

    /*!
     * \brief Получение фрейма из данного входного порта
     * \param frame Ссылка на фрейм
     * \param numSelfIn Номер входа
     */
    void ioRead(Frame& frame, int numSelfIn = 0) const;

    /*!
     * \brief Фреймов во входном буфере
     * \param numSelfIn Номер входа
     * \return Количество фреймов
     */
    int ioSize(int numSelfIn = 0) const;

    /*!
     * \brief Очистка входного буфера
     * \param numSelfIn Номер входа
     */
    void ioClear(int numSelfIn = 0);

    /*!
     * \brief Задать список доступных форматов входного порта
     * \param format Список форматов
     * \param numSelfIn Номер входного порта
     */
    void setAvaliableInputFormats(std::vector<Format> format, int numSelfIn = 0);

    /*!
     * \brief Задать список доступных форматов выходного порта
     * \param format Список форматов
     * \param numSelfOut Номер выходного порта
     */
    void setAvaliableOutputFormats(std::vector<Format> format, int numSelfOut = 0);

    /*!
     * \brief Задать интервал периодического вызова функции processing()
     * \param msec Интервал периодического вызова в миллисекундах
     * \param enabled Состояние вызова
     */
    void setProcessingTimeoutInterval(int msec, bool enabled = true);

    /*!
     * \brief Получить текущий интервал периодического вызова функции processing()
     * \param[out] msec Интервал периодического вызова в миллисекундах
     * \param[out] enabled Состояние вызова
     */
    void getProcessingTimeoutInterval(int& msec, bool& enabled);

    /*!
     * \brief Установить период для перехода в режим ожидания
     * \param msec Период в мсек
     */
    void setStandbyTimeout(int msec);

    /*!
     * \brief Текущий период для перехода в режим ожидания
     * \return Период в мсек
     */
    int getStandbyTimeout() const;

private:
    //обертка вокруг процессинга для выполнения в отдельном потоке
    void thread_execute();

    void timer_execute();

    //проверка статуса ожидания
    void check_standby();

private:
    //управление потоком обработки
    std::atomic_bool m_started;                 ///< статус активности устройства
    std::atomic_int m_activate_counter;         ///< счетчик готовности обработки
    std::condition_variable m_activate_check;   ///< условная переменная для пробуждения
    std::mutex m_exec_lock;   ///< мьютекс для защиты во время цикла обработки

    std::atomic_bool m_standby;
    int m_standby_msec;

    //входные/выходные подключения
    std::vector<PortInput*> m_inputs;
    std::vector<PortOutput*> m_outputs;

    //последняя точка активности
    std::chrono::system_clock::time_point m_last_active;

    //таймер для периодического вызова функции processing()
    std::shared_ptr<Timer> m_timer;
    int m_timeout;
    bool m_use_timer;
};

}   // namespace DspLab
