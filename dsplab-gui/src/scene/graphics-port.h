#pragma once

#include <QGraphicsItem>
#include <QGraphicsSceneMouseEvent>

#include <QPen>
#include <QSet>

#include <scene/i_graphics-port.h>
#include <scene/scene-tool-tip.h>

class QPainter;

/*!
 * \brief Графическая абстракция порта устройства
 */
class GraphicsPort
  : public QGraphicsObject
  , public IGraphicsPort
{
    Q_OBJECT

public:
    explicit GraphicsPort(IGraphicsDevice* parent, int num, IGraphicsPort::IOType iotype);
    virtual ~GraphicsPort();

    //диаметр графического элемента
    void setDiameter(qreal value);
    qreal getDiameter() const;

    //настройка подсветки
    void setSwitched(bool enabled);
    bool getSwitched() const;

    //фиксация подсветки
    void setFixedSwitched(bool enabled);
    bool getFixedSwitched() const;

    quint32 id() const final;
    IOType iotype() const final;
    int num() const final;

    //основные функции коммутации
    void attach(IGraphicsPort* port) final;
    void detach(IGraphicsPort* port) final;

    IGraphicsDevice* parent() const final;
    QPointF pos() const final;
    std::set<IGraphicsPort*> connections() const final;

    void setAvailableFormats(int flags) final;
    int availableFormats() const final;

    //активная область
    QRectF boundingRect() const final;

    //перегруженная функция рисования
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) final;

    //события входа/выхода курсора
    void hoverEnterEvent(QGraphicsSceneHoverEvent* event) final;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent* event) final;
    void mousePressEvent(QGraphicsSceneMouseEvent* event) final;

signals:

    //сигнал о разрыве (для объектов "соединений")
    void sgDetached(GraphicsPort* port);

    //сигнал о нажатии левой кнопки над объектом
    void sgLeftMouseButtonPressed();

private:
    IGraphicsDevice* m_parent;
    SceneToolTip m_tooltip;
    bool m_switched;
    IOType m_type;
    int m_num;
    quint32 m_id;
    int m_formats;

    //размеры элемента
    qreal m_posX;
    qreal m_posY;
    qreal m_width;
    qreal m_heigth;

    //параметры для отрисовки
    QPen m_pen;
    QBrush m_brush;

    std::set<IGraphicsPort*> m_connections;

    void updateToolTipPos();
};
