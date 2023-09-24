#pragma once

#include <dsplab/i_port-input.h>
#include <dsplab/i_port-output.h>

#include <mutex>
#include <list>

namespace DspLab {

class PortOutput;

/*!
 * \brief Абстракция входной точки подключения
 */
class PortInput : public IPortInput
{
public:
    friend class PortOutput;
    PortInput(IDevice* parent);
    virtual ~PortInput();

    IDevice* parent() const final;
    std::vector<Format> avaliableFormats() const final;
    void write(const Frame& frame) final;
    bool attach(IPortOutput* port) final;
    bool detach(IPortOutput* port) final;
    void detachAll() final;
    std::vector<IPortOutput*> connections() final;

    /*!
     * \brief Задать список совместимых форматов
     * \param format
     */
    void setAvaliableFormats(std::vector<Format> format);

    /*!
     * \brief Чтение из порта
     * \param frame Ссылка на объект фрейма
     */
    void frameRead(Frame& frame);

    /*!
     * \brief Количество фреймов в буфере порта
     * \return Количество фреймов
     */
    int frameCount() const;

    /*!
     * \brief Очистить буфер фреймов
     */
    void frameClear();

private:
    //совместимость форматов портов
    bool _portCompatibility(PortOutput* port);

    //проверка поддерживаемости формата
    bool _formatSupported(const Format& format);

    //разрыв соединений
    void _detachAll();

    //очистка буфера
    void _clearBuffer();

private:
    IDevice* m_parent;
    mutable std::mutex m_mutex;
    std::list<Frame> m_buffer;
    std::vector<Format> m_formats;
    std::vector<IPortOutput*> m_connections;
};

}   // namespace DspLab
