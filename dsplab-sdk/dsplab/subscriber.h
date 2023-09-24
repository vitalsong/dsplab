#ifndef SUBSCRIBER_H
#define SUBSCRIBER_H

#include <dsplab/event.h>

namespace DspLab {

class Subscriber
{
public:
    virtual ~Subscriber()
    {}
    virtual void update(Event event) = 0;
};

}   // namespace DspLab

#endif   // SUBSCRIBER_H
