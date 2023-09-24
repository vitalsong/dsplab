#include "scene-tool-tip.h"

#include <QPainter>

//-------------------------------------------------------------------------------------------------
SceneToolTip::SceneToolTip()
{
    m_item.setParentItem(this);
}

//-------------------------------------------------------------------------------------------------
void SceneToolTip::setText(QStringList text)
{
    m_text = text;

    QString str;
    for (const QString& it : m_text) {
        str += it + QString('\n');
    }

    m_item.setPlainText(str);

    QGraphicsItem::update();
}

//-------------------------------------------------------------------------------------------------
void SceneToolTip::setFont(QFont font)
{
    m_font = font;
    QGraphicsItem::update();
}

//-------------------------------------------------------------------------------------------------
void SceneToolTip::setBackgroundColor(QColor color)
{
    m_color = color;
    QGraphicsItem::update();
}

//-------------------------------------------------------------------------------------------------
QRectF SceneToolTip::boundingRect() const
{
    //расчет по тексту
    QFontMetrics metrics(m_font);
    qreal h = (m_text.count() + 1) * metrics.height();

    //поиск максимальной ширины
    qreal maxLength = 0;
    for (const QString& str : m_text) {
        qreal length = metrics.width(str);
        maxLength = (length > maxLength) ? (length) : (maxLength);
    }

    qreal w = maxLength * 1.3;

    return QRectF(0, 0, w, h);
}

//-------------------------------------------------------------------------------------------------
void SceneToolTip::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    painter->setPen(Qt::black);
    painter->setBrush(m_color);

    //основной прямоугольник модуля
    painter->drawRect(this->boundingRect());
}
