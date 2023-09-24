#include "main-view.h"
#include <QMimeData>
#include <QDebug>

//-------------------------------------------------------------------------------------------------
MainView::MainView(QWidget* parent)
  : QGraphicsView(parent)
{
    this->setAcceptDrops(true);
    this->setDragMode(QGraphicsView::ScrollHandDrag);
}

//-------------------------------------------------------------------------------------------------
void MainView::dragLeaveEvent(QDragLeaveEvent* event)
{
    event->ignore();
}

//-------------------------------------------------------------------------------------------------
void MainView::dragEnterEvent(QDragEnterEvent* event)
{
    event->acceptProposedAction();
}

//-------------------------------------------------------------------------------------------------
void MainView::dragMoveEvent(QDragMoveEvent* event)
{
    event->acceptProposedAction();
}

//-------------------------------------------------------------------------------------------------
void MainView::dropEvent(QDropEvent* event)
{
    const QMimeData* mimeData = event->mimeData();
    if (mimeData->objectName() == "DspLabDev") {
        event->acceptProposedAction();
        QString plugin = event->mimeData()->text();

        //позиция курсора
        QPointF pos = this->mapToScene(event->pos());

        //калибровка позиции для создания (относительно размера)
        //ЗЫ: временный костыль
        pos = QPointF(pos.x() + 67, pos.y() + 37);

        emit createDragDropDevice(plugin, pos);
    } else {
        event->ignore();
    }
}
