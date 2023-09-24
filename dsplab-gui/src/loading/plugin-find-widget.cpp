#include "plugin-find-widget.h"
#include <scene/dragdrop-device.h>

#include <QMouseEvent>
#include <QDrag>
#include <QMimeData>

//-------------------------------------------------------------------------------------------------
PluginFindWidget::PluginFindWidget(QWidget* parent)
  : QListWidget(parent)
  , m_device(new DragDropDevice)
{
    //nothing to do
}

//-------------------------------------------------------------------------------------------------
PluginFindWidget::~PluginFindWidget()
{
    delete m_device;
}

//-------------------------------------------------------------------------------------------------
void PluginFindWidget::mousePressEvent(QMouseEvent* event)
{
    QListWidget::mousePressEvent(event);
    if (event->button() == Qt::LeftButton) {
        //определяем элемент по координатам
        QListWidgetItem* item = this->itemAt(event->pos());
        if (item != nullptr) {
            //обработка события Drag для создания плагина
            QDrag* drag = new QDrag(this);
            QMimeData* mimeData = new QMimeData;

            QString text = item->text();
            m_device->setNamePlugin(text);
            mimeData->setText(text);
            mimeData->setObjectName("DspLabDev");

            QPixmap p(m_device->grab());
            drag->setMimeData(mimeData);
            drag->setPixmap(p);
            drag->exec(Qt::CopyAction | Qt::MoveAction, Qt::CopyAction);
        }
    }
}

//-------------------------------------------------------------------------------------------------
void PluginFindWidget::dragEnterEvent(QDragEnterEvent* event)
{
    event->ignore();
}

//-------------------------------------------------------------------------------------------------
void PluginFindWidget::dragMoveEvent(QDragMoveEvent* event)
{
    event->ignore();
}

//-------------------------------------------------------------------------------------------------
void PluginFindWidget::dropEvent(QDropEvent* event)
{
    event->ignore();
}
