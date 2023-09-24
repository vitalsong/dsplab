#include <scene/graphics-connection.h>
#include <scene/graphics-port.h>

#include <QPainter>

//-------------------------------------------------------------------------------------------------
GraphicsConnection::GraphicsConnection(GraphicsPort* first, GraphicsPort* second)
  : m_firstPortPtr(first)
  , m_secondPortPtr(second)
{
    //----------------------------------------------------------------------
    //линия не будет существовать, если хотя бы один связующий объект удален
    //----------------------------------------------------------------------

    //реакция на удаление
    connect(m_firstPortPtr, SIGNAL(destroyed()), this, SLOT(onDeleteItem()));

    connect(m_secondPortPtr, SIGNAL(destroyed()), this, SLOT(onDeleteItem()));

    //реакция на изменение позиции (родителя)
    connect(m_firstPortPtr->parentObject(), SIGNAL(xChanged()), this, SLOT(onChangePos()));

    connect(m_firstPortPtr->parentObject(), SIGNAL(yChanged()), this, SLOT(onChangePos()));

    connect(m_secondPortPtr->parentObject(), SIGNAL(xChanged()), this, SLOT(onChangePos()));

    connect(m_secondPortPtr->parentObject(), SIGNAL(yChanged()), this, SLOT(onChangePos()));

    //реакция на отключение
    connect(m_firstPortPtr, SIGNAL(sgDetached(GraphicsPort*)), this, SLOT(onDetach(GraphicsPort*)));

    connect(m_secondPortPtr, SIGNAL(sgDetached(GraphicsPort*)), this, SLOT(onDetach(GraphicsPort*)));
}

//-------------------------------------------------------------------------------------------------
GraphicsConnection::~GraphicsConnection()
{
    //nothing to do
}

//-------------------------------------------------------------------------------------------------
QRectF GraphicsConnection::boundingRect() const
{
    //если хотя бы один объект был удален
    if (!m_firstPortPtr || !m_secondPortPtr) {
        return m_rect;
    }

    QPointF sp0;
    QPointF sp1 = m_firstPortPtr->scenePos();
    QPointF sp2 = m_secondPortPtr->scenePos();

    //определяем относительное положение точек
    if ((sp1.x() <= sp2.x()) && (sp1.y() <= sp2.y())) {
        sp0.setX(sp1.x());
        sp0.setY(sp1.y());
    } else if ((sp1.x() <= sp2.x()) && (sp1.y() >= sp2.y())) {
        sp0.setX(sp1.x());
        sp0.setY(sp2.y());
    } else if ((sp2.x() <= sp1.x()) && (sp2.y() <= sp1.y())) {
        sp0.setX(sp2.x());
        sp0.setY(sp2.y());
    } else if ((sp2.x() <= sp1.x()) && (sp2.y() >= sp1.y())) {
        sp0.setX(sp2.x());
        sp0.setY(sp1.y());
    }

    //определяем прямоугольную область рисования
    m_rect.setX(sp0.x());
    m_rect.setY(sp0.y());
    m_rect.setWidth(qAbs(sp1.x() - sp2.x()));
    m_rect.setHeight(qAbs(sp1.y() - sp2.y()));

    return m_rect;
}

//-------------------------------------------------------------------------------------------------
void GraphicsConnection::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    //проверка существования объектов связи
    if (!m_firstPortPtr || !m_secondPortPtr) {
        return;
    }

    painter->setPen(Qt::white);
    painter->setBrush(Qt::white);

    QPointF sp1 = m_firstPortPtr->scenePos();
    QPointF sp2 = m_secondPortPtr->scenePos();

    qreal x0 = sp1.x();
    qreal y0 = sp1.y();

    qreal x1 = x0 - (sp1.x() - sp2.x()) / 2;
    qreal y1 = sp1.y();
    painter->drawLine(x0, y0, x1, y1);

    qreal x2 = x1;
    qreal y2 = sp2.y();
    painter->drawLine(x1, y1, x2, y2);

    qreal x3 = sp2.x();
    qreal y3 = sp2.y();
    painter->drawLine(x2, y2, x3, y3);
}

//-------------------------------------------------------------------------------------------------
void GraphicsConnection::onChangePos()
{
    QGraphicsItem::prepareGeometryChange();
}

//-------------------------------------------------------------------------------------------------
void GraphicsConnection::onDeleteItem()
{
    m_firstPortPtr = nullptr;
    m_secondPortPtr = nullptr;

    this->deleteLater();
}

//-------------------------------------------------------------------------------------------------
void GraphicsConnection::onDetach(GraphicsPort* port)
{
    //если данное соединение было разорвано
    if (sender() == m_firstPortPtr || sender() == m_secondPortPtr) {
        if (port == m_firstPortPtr || port == m_secondPortPtr) {
            this->onDeleteItem();
        }
    }
}
