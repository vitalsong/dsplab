#include "plugin-tree-widget.h"
#include <scene/dragdrop-device.h>

#include <QMouseEvent>
#include <QDrag>
#include <QMimeData>

//-------------------------------------------------------------------------------------------------
PluginTreeWidget::PluginTreeWidget(QWidget* parent)
  : QTreeWidget(parent)
  , m_device(new DragDropDevice)
{
    this->setHeaderHidden(true);
}

//-------------------------------------------------------------------------------------------------
PluginTreeWidget::~PluginTreeWidget()
{
    delete m_device;
}

//-------------------------------------------------------------------------------------------------
void PluginTreeWidget::mousePressEvent(QMouseEvent* event)
{
    QTreeWidget::mousePressEvent(event);
    if (event->button() == Qt::LeftButton) {
        //определяем элемент по координатам
        QTreeWidgetItem* item = this->itemAt(event->pos());
        if (item != nullptr) {
            //если объект списка представляет плагин (а не группу)
            if (item->parent() != nullptr) {
                //обработка события Drag для создания плагина
                QDrag* drag = new QDrag(this);
                QMimeData* mimeData = new QMimeData;

                QString text = item->text(0);
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
}

//-------------------------------------------------------------------------------------------------
void PluginTreeWidget::dragEnterEvent(QDragEnterEvent* event)
{
    event->ignore();
}

//-------------------------------------------------------------------------------------------------
void PluginTreeWidget::dragMoveEvent(QDragMoveEvent* event)
{
    event->ignore();
}

//-------------------------------------------------------------------------------------------------
void PluginTreeWidget::dropEvent(QDropEvent* event)
{
    event->ignore();
}
