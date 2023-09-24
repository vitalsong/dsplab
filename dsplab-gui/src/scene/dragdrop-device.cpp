#include "dragdrop-device.h"
#include <QPainter>
#include <QPaintEvent>

namespace {
const int WIDTH = 120;
const int HEIGTH = 60;
const QColor PEN_COLOR = Qt::black;
const QColor BRUSH_COLOR = Qt::white;
}   // namespace

//-------------------------------------------------------------------------------------------------
DragDropDevice::DragDropDevice(QWidget* parent)
  : QWidget(parent)
  , m_layout(new QHBoxLayout(this))
  , m_label(new QLabel(this))
{
    //убираем панель "свернуть, закрыть"
    this->setWindowFlags(Qt::Popup);

    //добавляем текст на слой
    this->layout()->addWidget(m_label);

    //настройка текста
    m_label->setFont(QFont("Times", 10, QFont::Bold));
}

//-------------------------------------------------------------------------------------------------
void DragDropDevice::setNamePlugin(const QString& text)
{
    //устанавливаем текст
    m_label->setText(text);

    //подгоняем размер под размер текста
    int width = std::max(m_label->width(), WIDTH);
    this->resize(width, HEIGTH);
}

//-------------------------------------------------------------------------------------------------
QString DragDropDevice::getNamePlugin() const
{
    return m_label->text();
}

//-------------------------------------------------------------------------------------------------
void DragDropDevice::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    painter.setPen(PEN_COLOR);
    painter.setBrush(BRUSH_COLOR);
    int width = this->width();
    int height = this->height();
    painter.drawRect(QRectF(1, 1, width - 2, height - 2));
}
