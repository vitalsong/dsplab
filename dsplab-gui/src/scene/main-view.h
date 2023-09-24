#pragma once

#include <QGraphicsView>
#include <QDragEnterEvent>
#include <QDragMoveEvent>
#include <QDropEvent>

/*!
 * \brief Класс для поддержки drug and drop на сцене
 */
class MainView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit MainView(QWidget* parent);

signals:
    /*! сигнал добавления объекта на сцену */
    void createDragDropDevice(QString, QPointF);

protected:
    /*! основные события DrapDrop при перемещении объекта девайса */   // see doc Qt::DragDrop
    void dragLeaveEvent(QDragLeaveEvent* event);
    void dragEnterEvent(QDragEnterEvent* event);
    void dragMoveEvent(QDragMoveEvent* event);
    void dropEvent(QDropEvent* event);
};
