#include <scene/graphics-port.h>
#include <scene/graphics-connection.h>

#include <QGraphicsScene>
#include <QPainter>
#include <QCursor>

//-------------------------------------------------------------------------------------------------
GraphicsPort::GraphicsPort(IGraphicsDevice* parent, int num, IGraphicsPort::IOType type)
  : m_switched(false)
  , m_type(type)
  , m_num(num)
  , m_posX(-8)
  , m_posY(-8)
  , m_width(16)
  , m_heigth(16)
  , m_pen(Qt::black, 2)
  , m_brush(Qt::white)
{
    static quint32 g_id = 0;
    m_id = ++g_id;
    m_parent = parent;

    //разрешаем обработку событий ввода в область
    this->setAcceptHoverEvents(true);

    //настройка отображения информации
    m_tooltip.setBackgroundColor(QColor(253, 247, 208));
    m_tooltip.setFont(QFont("Times", 8, QFont::Normal));
    m_tooltip.setParentItem(this);
    m_tooltip.hide();

    //настройка расположения описателя
    this->updateToolTipPos();
}

//-------------------------------------------------------------------------------------------------
GraphicsPort::~GraphicsPort()
{
    //nothing to do
}

//-------------------------------------------------------------------------------------------------
void GraphicsPort::setDiameter(qreal value)
{
    m_width = value;
    m_heigth = value;
}

//-------------------------------------------------------------------------------------------------
qreal GraphicsPort::getDiameter() const
{
    return m_width;
}

//-------------------------------------------------------------------------------------------------
void GraphicsPort::setSwitched(bool enabled)
{
    if (m_switched) {
        return;
    }

    if (enabled) {
        m_brush.setColor(Qt::black);
    } else {
        m_brush.setColor(Qt::white);
    }

    //перерисовка объекта
    this->update();
}

//-------------------------------------------------------------------------------------------------
bool GraphicsPort::getSwitched() const
{
    return (m_brush.color() == Qt::black);
}

//-------------------------------------------------------------------------------------------------
void GraphicsPort::setFixedSwitched(bool enabled)
{
    m_switched = enabled;
}

//-------------------------------------------------------------------------------------------------
bool GraphicsPort::getFixedSwitched() const
{
    return m_switched;
}

//-------------------------------------------------------------------------------------------------
quint32 GraphicsPort::id() const
{
    return m_id;
}

//-------------------------------------------------------------------------------------------------
IGraphicsPort::IOType GraphicsPort::iotype() const
{
    return m_type;
}

//-------------------------------------------------------------------------------------------------
int GraphicsPort::num() const
{
    return m_num;
}

//-------------------------------------------------------------------------------------------------
void GraphicsPort::attach(IGraphicsPort* port)
{
    //костыль, я знаю
    GraphicsPort* gport = dynamic_cast<GraphicsPort*>(port);

    //создаем объект соединения
    GraphicsConnection* path = new GraphicsConnection(this, gport);

    //добавляем в список
    gport->m_connections.insert(this);
    m_connections.insert(port);

    //добавляем объект на сцену родителя
    path->setZValue(0);
    this->scene()->addItem(path);
}

//-------------------------------------------------------------------------------------------------
void GraphicsPort::detach(IGraphicsPort* port)
{
    //костыль, я знаю
    GraphicsPort* gport = dynamic_cast<GraphicsPort*>(port);

    //удаляем из списков
    gport->m_connections.erase(this);
    m_connections.erase(port);

    //сообщаем о отключении
    emit sgDetached(gport);
}

//-------------------------------------------------------------------------------------------------
IGraphicsDevice* GraphicsPort::parent() const
{
    return m_parent;
}

//-------------------------------------------------------------------------------------------------
QPointF GraphicsPort::pos() const
{
    return QGraphicsObject::scenePos();
}

//-------------------------------------------------------------------------------------------------
std::set<IGraphicsPort*> GraphicsPort::connections() const
{
    return m_connections;
}

//-------------------------------------------------------------------------------------------------
void GraphicsPort::setAvailableFormats(int flags)
{
    m_formats = flags;
    QStringList text;

    if (m_formats & Real) {
        text.push_back("real");
    }

    if (m_formats & Complex) {
        text.push_back("complex");
    }

    if (m_formats & Binary) {
        text.push_back("binary");
    }

    m_tooltip.setText(text);
    this->updateToolTipPos();
}

//-------------------------------------------------------------------------------------------------
int GraphicsPort::availableFormats() const
{
    return m_formats;
}

//-------------------------------------------------------------------------------------------------
QRectF GraphicsPort::boundingRect() const
{
    return QRectF(m_posX, m_posY, m_width, m_heigth);
}

//-------------------------------------------------------------------------------------------------
void GraphicsPort::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    painter->setPen(m_pen);
    painter->setBrush(m_brush);
    painter->drawEllipse(this->boundingRect());
}

//-------------------------------------------------------------------------------------------------
void GraphicsPort::hoverEnterEvent(QGraphicsSceneHoverEvent* event)
{
    Q_UNUSED(event);
    this->setCursor(QCursor(Qt::PointingHandCursor));
    this->setSwitched(true);
    m_tooltip.show();
}

//-------------------------------------------------------------------------------------------------
void GraphicsPort::hoverLeaveEvent(QGraphicsSceneHoverEvent* event)
{
    Q_UNUSED(event);
    this->setCursor(QCursor(Qt::ArrowCursor));
    this->setSwitched(false);
    m_tooltip.hide();
}

//-------------------------------------------------------------------------------------------------
void GraphicsPort::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    m_tooltip.hide();
    QGraphicsObject::mousePressEvent(event);
}

//-------------------------------------------------------------------------------------------------
void GraphicsPort::updateToolTipPos()
{
    if (m_type == Input) {
        QPointF pos = {-m_tooltip.boundingRect().width() - 5, -m_tooltip.boundingRect().height() - 5};
        m_tooltip.setPos(pos);
    } else {
        QPointF pos = {m_tooltip.pos().x() + 5, m_tooltip.pos().y() + 5};
        m_tooltip.setPos(pos);
    }
}
