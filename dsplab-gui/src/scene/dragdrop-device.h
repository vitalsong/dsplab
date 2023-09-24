#pragma once

#include <QWidget>
#include <QLayout>
#include <QLabel>
#include <QString>

/*!
 * \brief Объект девайса при перемещении DragDrop
 */
class DragDropDevice : public QWidget
{
    Q_OBJECT
public:
    explicit DragDropDevice(QWidget* parent = nullptr);

    //установить\вернуть имя плагина
    void setNamePlugin(const QString& text);
    QString getNamePlugin() const;

protected:
    //отрисовка плагина
    void paintEvent(QPaintEvent*);

private:
    QLayout* m_layout;
    QLabel* m_label;
};
