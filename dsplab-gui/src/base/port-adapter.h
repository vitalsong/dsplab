#pragma once

#include <vector>
#include <set>

class DeviceFacade;
class IGraphicsPort;

namespace DspLab {
class IPortInput;
class IPortOutput;
}   // namespace DspLab

/*!
 * \brief Класс-адаптер порта подключения
 */
class PortAdapter
{
public:
    /*!
     * \brief Тип адаптера порта
     */
    enum Type
    {
        Input,
        Output
    };

    PortAdapter(DeviceFacade* parent, DspLab::IPortInput* port, IGraphicsPort* view, int num = 0);
    PortAdapter(DeviceFacade* parent, DspLab::IPortOutput* port, IGraphicsPort* view, int num = 0);
    virtual ~PortAdapter();

    DeviceFacade* parent() const;
    Type type() const;
    int num() const;
    bool attach(PortAdapter* ioport);
    bool detach(PortAdapter* ioport);
    void detachAll();
    std::set<PortAdapter*> connections() const;

private:
    Type m_type;
    int m_num;
    IGraphicsPort* m_view;
    DeviceFacade* m_parent;
    DspLab::IPortInput* m_input;
    DspLab::IPortOutput* m_output;
    std::set<PortAdapter*> m_connections;

    //конфигурирование информации о форматах
    void configurateFormatsView();
};
