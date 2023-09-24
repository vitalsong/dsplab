#include <abstract-text-label.h>
#include <qwt_text_label.h>
#include <QEvent>

//-------------------------------------------------------------------------------------------------
AbstractTextLabel::AbstractTextLabel(QWidget *parent, Position pos) :
    QObject(parent),
    m_parent(parent)
{
    m_text = " ";
    m_pos = pos;
    m_label = new QwtTextLabel(parent);

    if (m_parent != Q_NULLPTR) {
        m_parent->installEventFilter(this);
    }
}

//-------------------------------------------------------------------------------------------------
AbstractTextLabel::~AbstractTextLabel()
{
    //nothing to do
}

//-------------------------------------------------------------------------------------------------
bool AbstractTextLabel::eventFilter(QObject *watched, QEvent *event)
{
    //если перехват не для полотна
    if (watched != m_parent) {
        return QObject::eventFilter(watched, event);
    }

    //если событие - изменение размеров виджета
    if (event->type() == QEvent::Resize) {
        this->repaint();
    }

    //возвращаем управление
    return QObject::eventFilter(watched, event);
}

//-------------------------------------------------------------------------------------------------
void AbstractTextLabel::repaint()
{
    QwtText qwt_text(m_text);
    qwt_text.setColor(Qt::white);
    qwt_text.setRenderFlags(Qt::AlignLeft);
    int text_height = qwt_text.textSize().height();
    int text_width = qwt_text.textSize().width();
    QPoint text_pos = _detectTextPos(text_height, text_width);
    m_label->setText(qwt_text);
    m_label->resize(text_width + 10, text_height + 10);
    m_label->move(text_pos);
}

//-------------------------------------------------------------------------------------------------
void AbstractTextLabel::setText(QString text)
{
    m_text = text;
}

//-------------------------------------------------------------------------------------------------
QString AbstractTextLabel::getText() const
{
    return m_text;
}

//-------------------------------------------------------------------------------------------------
void AbstractTextLabel::setPos(AbstractTextLabel::Position pos)
{
    m_pos = pos;
    this->repaint();
}

//-------------------------------------------------------------------------------------------------
AbstractTextLabel::Position AbstractTextLabel::getPos() const
{
    return m_pos;
}

//-------------------------------------------------------------------------------------------------
QPoint AbstractTextLabel::_detectTextPos(int text_height, int text_width)
{
    QPoint left_pos;
    QPoint right_pos;
    QPoint text_pos;

    switch (m_pos)
    {
    case TopLeft:
        left_pos = m_parent->rect().topLeft();
        text_pos = left_pos + QPoint(10, 10);
        break;

    case BottomLeft:
        left_pos = m_parent->rect().bottomLeft();
        text_pos = left_pos + QPoint(0, -text_height) + QPoint(10, -10);
        break;

    case TopRight:
        right_pos = m_parent->rect().topRight();
        text_pos = right_pos + QPoint(-text_width, 0) + QPoint(-20, 10);
        break;

    case BottomRight:
        right_pos = m_parent->rect().bottomRight();
        text_pos = right_pos + QPoint(-text_width, -text_height) + QPoint(-20, -10);
        break;
    }

    return text_pos;
}
