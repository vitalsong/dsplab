#pragma once

#include <QGraphicsItem>
#include <QCursor>

#include <scene/i_graphics-device.h>

#include <memory>

class QPainter;
class QGraphicsSceneMouseEvent;

/*!
 * \brief Графическая абстракция устройства
 */
class GraphicsDevice
  : public QGraphicsObject
  , public IGraphicsDevice
{
    Q_OBJECT

public:
    GraphicsDevice();
    virtual ~GraphicsDevice();

    quint32 id() const;

    void setNumInputs(int num);
    int getNumInputs() const;

    void setNumOutputs(int num);
    int getNumOutputs() const;

    IGraphicsPort* input(int num) const;
    IGraphicsPort* input(quint32 id) const;

    IGraphicsPort* output(int num) const;
    IGraphicsPort* output(quint32 id) const;

    void setStartedStatus(StartedStatus status);
    StartedStatus getStartedStatus() const;

    void setScenePos(QPointF pos);
    QPointF getScenePos() const;

    void setNameText(QString text);
    QString getNameText() const;

    void updateValuesList(QStringList list);

signals:
    void rightMouseButtonClicked(QPointF pos);
    void leftMouseDoubleClicked(QPointF pos);

private:
    //обновить надпись на элементе
    void updateTextLabel();

    //конфигурация точек входа/выхода
    void configSize();
    void configInputs();
    void configOutputs();

    //перегруженная функция получения активной области элемента
    QRectF boundingRect() const final;

    //перегруженная функция перерисовки данного элемента
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) final;

    //события удержания/отпускания клавиши
    void mousePressEvent(QGraphicsSceneMouseEvent* event) final;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) final;

    //события входа/выхода курсора
    void hoverEnterEvent(QGraphicsSceneHoverEvent* event) final;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent* event) final;

    //реакция на двойное нажатие
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event) final;

private:
    int m_nin;
    int m_nout;
    quint32 m_id;
    StartedStatus m_status;
    QString m_name;
    int m_maxValuesCount;

    //графические объекты входа/выхода
    typedef std::shared_ptr<IGraphicsPort> IGraphicsPortPtr;
    QList<IGraphicsPortPtr> m_inputs;
    QList<IGraphicsPortPtr> m_outputs;

    //размеры основного объекта
    qreal m_posx, m_posy, m_width, m_heigth, m_diameter;

    //элемент текста
    QGraphicsTextItem* m_label;
    QGraphicsTextItem* m_display;
    QColor m_border_color;
};
