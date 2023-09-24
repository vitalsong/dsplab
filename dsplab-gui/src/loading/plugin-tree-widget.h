#pragma once

#include <QTreeWidget>

class DragDropDevice;

/*!
 * \brief Класс виджет плагинов с реализацией DragDrop
 */
class PluginTreeWidget : public QTreeWidget
{
    Q_OBJECT

public:
    explicit PluginTreeWidget(QWidget* parent = nullptr);
    ~PluginTreeWidget();

protected:
    void mousePressEvent(QMouseEvent* event);

    /*! основные события DragDrop в которых игнорится перемещение */
    void dragEnterEvent(QDragEnterEvent* event);
    void dragMoveEvent(QDragMoveEvent* event);
    void dropEvent(QDropEvent* event);

private:
    DragDropDevice* m_device;
};
