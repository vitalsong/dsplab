#pragma once

#include <QPointF>
#include <set>

class IGraphicsDevice;

class IGraphicsPort
{
public:
    virtual ~IGraphicsPort()
    {}

    enum IOType
    {
        Input,
        Output
    };

    virtual quint32 id() const = 0;
    virtual IOType iotype() const = 0;
    virtual int num() const = 0;
    virtual void attach(IGraphicsPort* ioport) = 0;
    virtual void detach(IGraphicsPort* ioport) = 0;
    virtual IGraphicsDevice* parent() const = 0;
    virtual QPointF pos() const = 0;
    virtual std::set<IGraphicsPort*> connections() const = 0;

    enum Format
    {
        Real = 0x01,
        Complex = 0x02,
        Binary = 0x04
    };

    virtual void setAvailableFormats(int flags) = 0;
    virtual int availableFormats() const = 0;
};
