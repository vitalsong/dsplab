#pragma once

#include <QString>
#include <QPointF>

#include <scene/i_graphics-port.h>

/*!
 * \brief Интерфейс графического объекта
 */
class IGraphicsDevice
{
public:
    virtual ~IGraphicsDevice()
    {}

    //идентификатор объекта
    virtual quint32 id() const = 0;

    //конфигурирование портов
    virtual void setNumInputs(int num) = 0;
    virtual int getNumInputs() const = 0;

    virtual void setNumOutputs(int num) = 0;
    virtual int getNumOutputs() const = 0;

    virtual IGraphicsPort* input(int num) const = 0;
    virtual IGraphicsPort* input(quint32 id) const = 0;

    virtual IGraphicsPort* output(int num) const = 0;
    virtual IGraphicsPort* output(quint32 id) const = 0;

    //состояние запуска
    enum StartedStatus
    {
        Started,
        Stopped,
        Waiting
    };

    virtual void setStartedStatus(StartedStatus status) = 0;
    virtual StartedStatus getStartedStatus() const = 0;

    //текущие координаты на сцене
    virtual void setScenePos(QPointF pos) = 0;
    virtual QPointF getScenePos() const = 0;

    //отображение имени
    virtual void setNameText(QString text) = 0;
    virtual QString getNameText() const = 0;

    //отображение списка переменных
    virtual void updateValuesList(QStringList list) = 0;
};
