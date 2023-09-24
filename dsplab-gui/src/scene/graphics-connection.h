#pragma once

#include <QGraphicsItem>

class QPainter;
class GraphicsPort;

/*!
 * \brief Графическая абстракция соединения между портами
 */
class GraphicsConnection : public QGraphicsObject
{
    Q_OBJECT

public:
    explicit GraphicsConnection(GraphicsPort* first, GraphicsPort* second);
    virtual ~GraphicsConnection();

private:
    //указатели на объекты портов для соединения
    GraphicsPort* m_firstPortPtr;
    GraphicsPort* m_secondPortPtr;

    //текущая область перерисовки
    mutable QRectF m_rect;

    //определение области рисования
    QRectF boundingRect() const;

    //перерисовка объекта
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) final;

private slots:
    void onChangePos();
    void onDeleteItem();
    void onDetach(GraphicsPort* port);
};
