#pragma once

#include <QGraphicsItem>
#include <QGraphicsTextItem>
#include <QFont>

class SceneToolTip : public QGraphicsItem
{
public:
    SceneToolTip();
    void setText(QStringList text);
    void setFont(QFont font);
    void setBackgroundColor(QColor color);

    //перегрузка базовых функций
    QRectF boundingRect() const final;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) final;

private:
    QStringList m_text;
    QFont m_font;
    QColor m_color;
    QGraphicsTextItem m_item;
};
