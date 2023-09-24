#pragma once

#include <QListWidget>

class DragDropDevice;

/*!
 * \brief Класс виджет поиска плагинов с реализацией DragDrop
 */
class PluginFindWidget : public QListWidget
{
    Q_OBJECT

public:
    explicit PluginFindWidget(QWidget* parent = nullptr);
    ~PluginFindWidget();

protected:
    void mousePressEvent(QMouseEvent* event);

    /*! основные события DragDrop в которых игнорится перемещение */
    void dragEnterEvent(QDragEnterEvent* event);
    void dragMoveEvent(QDragMoveEvent* event);
    void dropEvent(QDropEvent* event);

private:
    DragDropDevice* m_device;
};
