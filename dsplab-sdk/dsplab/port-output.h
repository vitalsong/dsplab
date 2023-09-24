#pragma once

#include <dsplab/i_port-input.h>
#include <dsplab/i_port-output.h>

#include <mutex>
#include <vector>

namespace DspLab {

class PortInput;

/*!
 * \brief Абстракция выходной точки подключения
 */
class PortOutput : public IPortOutput
{
public:
    friend class PortInput;
    PortOutput(IDevice* parent);
    virtual ~PortOutput();

    IDevice* parent() const final;
    std::vector<Format> avaliableFormats() const final;
    bool attach(IPortInput* port) final;
    bool detach(IPortInput* port) final;
    void detachAll() final;
    std::vector<IPortInput*> connections() final;

    /*!
     * \brief Задать список совместимых форматов
     * \param format
     */
    void setAvaliableFormats(std::vector<Format> format);

private:
    bool _portCompatibility(PortInput* port);
    void _detachAll();

private:
    IDevice* m_parent;
    mutable std::mutex m_mutex;
    std::vector<Format> m_formats;
    std::vector<IPortInput*> m_connections;
};

}   // namespace DspLab
