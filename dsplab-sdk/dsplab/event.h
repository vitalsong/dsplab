#ifndef EVENT_H
#define EVENT_H

#include <string>

namespace DspLab {

class Publisher;

/*!
 * \brief Класс события
 */
class Event
{
public:
    /*!
     * \brief Тип события
     */
    enum EventType
    {
        DeviceStarted,
        DeviceStopped,
        DeviceStandbyOn,
        DeviceStandbyOff,
        DeviceError
    };

    Event(EventType type, Publisher* publisher, std::string msg = {})
      : m_type(type)
      , m_publisher(publisher)
      , m_msg(msg)
    {
        //nothing to do
    }

    EventType type() const
    {
        return m_type;
    }

    Publisher* publisher() const
    {
        return m_publisher;
    }

    const std::string& message() const
    {
        return m_msg;
    }

private:
    EventType m_type;
    Publisher* m_publisher;
    std::string m_msg;
};

}   // namespace DspLab

#endif   // EVENT_H
