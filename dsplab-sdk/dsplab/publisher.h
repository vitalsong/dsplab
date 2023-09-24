#ifndef PUBLISHER_H
#define PUBLISHER_H

#include <dsplab/subscriber.h>

#include <set>
#include <mutex>

namespace DspLab {

/*!
 * \brief Реализация класса "издатель"
 */
class Publisher
{
public:
    virtual ~Publisher()
    {
        std::lock_guard<std::mutex> locker(m_mutex);
    }

    virtual void subscribe(Subscriber* subscriber)
    {
        std::lock_guard<std::mutex> locker(m_mutex);
        m_subscribers.insert(subscriber);
    }

    virtual void unsubscribe(Subscriber* subscriber)
    {
        std::lock_guard<std::mutex> locker(m_mutex);
        m_subscribers.erase(subscriber);
    }

    virtual void notify(Event event)
    {
        std::lock_guard<std::mutex> locker(m_mutex);
        for (auto subscriber : m_subscribers) {
            subscriber->update(event);
        }
    }

private:
    std::mutex m_mutex;
    std::set<Subscriber*> m_subscribers;
};

}   // namespace DspLab

#endif   // PUBLISHER_H
