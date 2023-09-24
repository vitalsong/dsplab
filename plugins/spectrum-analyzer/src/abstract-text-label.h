#ifndef QWTTEXTLABEL_H
#define QWTTEXTLABEL_H

#include <QObject>

class QwtTextLabel;

/*!
 * \brief Абстрактный класс для отрисовки текста
 */
class AbstractTextLabel : public QObject
{
    Q_OBJECT

public:
    enum Position
    {
        TopLeft,
        BottomLeft,
        TopRight,
        BottomRight
    };

    AbstractTextLabel(QWidget* parent, Position pos);
    virtual ~AbstractTextLabel();

    //перехват изменения размеров
    virtual bool eventFilter(QObject *watched, QEvent *event);

    //перерисовка
    virtual void repaint();

    //текст для вывода
    void setText(QString text);
    QString getText() const;

    //позиция для отображения
    void setPos(Position pos);
    Position getPos() const;

    //видимость объекта
    void setVisible(bool visible);
    bool isVisible() const;

private:
    //определить положение текста (по текущему размеру окна и режиму отображения)
    QPoint _detectTextPos(int text_height, int text_width);

private:
    QString m_text;
    Position m_pos;
    QwtTextLabel* m_label;
    QWidget* m_parent;
};

#endif // QWTTEXTLABEL_H
