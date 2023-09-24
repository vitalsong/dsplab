#include <scene/graphics-device.h>
#include <scene/graphics-port.h>

#include <QPainter>
#include <QGraphicsSceneEvent>

static const int MAX_HEIGTH = 120;
static const int MAX_WIDTH = 240;
static const int SHADOW_WIDTH = 7;
static quint32 g_id = 0;

//-------------------------------------------------------------------------------------------------
GraphicsDevice::GraphicsDevice()
  : m_nin(1)
  , m_nout(1)
  , m_status(IGraphicsDevice::Stopped)
  , m_name("<Device>")
  , m_maxValuesCount(1)
  , m_posx(-60)
  , m_posy(-30)
  , m_width(120)
  , m_heigth(60)
  , m_diameter(16)
  , m_border_color(Qt::black)
{
    //идентификация
    m_id = ++g_id;

    //разрешаем обработку событий вхождения в область
    this->setAcceptHoverEvents(true);
    this->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);

    //отображение имени и переменных
    m_label = new QGraphicsTextItem(this);
    m_display = new QGraphicsTextItem(this);

    this->updateTextLabel();

    //конфигурируем размер
    this->configSize();

    //размещаем точки входа
    this->configInputs();

    //размещаем точки выхода
    this->configOutputs();
}

//-------------------------------------------------------------------------------------------------
GraphicsDevice::~GraphicsDevice()
{
    //nothing to do
}

//-------------------------------------------------------------------------------------------------
quint32 GraphicsDevice::id() const
{
    return m_id;
}

//-------------------------------------------------------------------------------------------------
void GraphicsDevice::setNumInputs(int num)
{
    m_nin = num;

    //обновление размеров
    this->configSize();

    //размещаем точки входа
    this->configInputs();

    QGraphicsObject::update();
}

//-------------------------------------------------------------------------------------------------
int GraphicsDevice::getNumInputs() const
{
    return m_nin;
}

//-------------------------------------------------------------------------------------------------
void GraphicsDevice::setNumOutputs(int num)
{
    m_nout = num;

    //обновление размеров
    this->configSize();

    //размещаем точки выхода
    this->configOutputs();

    QGraphicsObject::update();
}

//-------------------------------------------------------------------------------------------------
int GraphicsDevice::getNumOutputs() const
{
    return m_nout;
}

//-------------------------------------------------------------------------------------------------
IGraphicsPort* GraphicsDevice::input(int num) const
{
    return m_inputs.at(num).get();
}

//-------------------------------------------------------------------------------------------------
IGraphicsPort* GraphicsDevice::input(quint32 id) const
{
    for (auto port : m_inputs) {
        if (port->id() == id) {
            return port.get();
        }
    }

    return nullptr;
}

//-------------------------------------------------------------------------------------------------
IGraphicsPort* GraphicsDevice::output(int num) const
{
    return m_outputs.at(num).get();
}

//-------------------------------------------------------------------------------------------------
IGraphicsPort* GraphicsDevice::output(quint32 id) const
{
    for (auto port : m_outputs) {
        if (port->id() == id) {
            return port.get();
        }
    }

    return nullptr;
}

//-------------------------------------------------------------------------------------------------
void GraphicsDevice::setStartedStatus(IGraphicsDevice::StartedStatus status)
{
    m_status = status;
    QGraphicsObject::update();
}

//-------------------------------------------------------------------------------------------------
IGraphicsDevice::StartedStatus GraphicsDevice::getStartedStatus() const
{
    return m_status;
}

//-------------------------------------------------------------------------------------------------
void GraphicsDevice::setScenePos(QPointF pos)
{
    QGraphicsObject::setPos(pos);
    QGraphicsObject::update();
}

//-------------------------------------------------------------------------------------------------
QPointF GraphicsDevice::getScenePos() const
{
    return QGraphicsObject::scenePos();
}

//-------------------------------------------------------------------------------------------------
void GraphicsDevice::setNameText(QString text)
{
    m_name = text;

    //обновление текста
    this->updateTextLabel();

    //обновление размеров
    this->configSize();

    QGraphicsObject::update();
}

//-------------------------------------------------------------------------------------------------
QString GraphicsDevice::getNameText() const
{
    return m_name;
}

//-------------------------------------------------------------------------------------------------
void GraphicsDevice::updateValuesList(QStringList list)
{
    if (list.size() > m_maxValuesCount) {
        m_maxValuesCount = list.size();

        //обновление размеров
        this->configSize();
    }

    m_display->setTextWidth(m_width - m_width / 5);
    m_display->setDefaultTextColor(Qt::black);
    m_display->setFont(QFont("Times", 9, QFont::Bold));
    m_display->setPos(m_posx + m_width / 5, m_posy + m_label->boundingRect().height());

    QString text;
    for (const QString& it : list) {
        text += it + QString('\n');
    }

    m_display->setPlainText(text);

    QGraphicsObject::update();
}

//-------------------------------------------------------------------------------------------------
void GraphicsDevice::updateTextLabel()
{
    m_label->setTextWidth(m_width - m_width / 5);
    m_label->setDefaultTextColor(Qt::black);
    m_label->setFont(QFont("Times", 10, QFont::Bold));
    m_label->setPos(m_posx + m_width / 5, m_posy);
    m_label->setPlainText(m_name);
}

//-------------------------------------------------------------------------------------------------
void GraphicsDevice::configSize()
{
    int nin = m_nin;
    int nout = m_nout;
    int valueCount = m_maxValuesCount;

    QFontMetrics metrics(m_display->font());

    //высота напрямую зависит от количества входов (выходов)
    //диаметра объекта "точки подключения" и количества переменных отображения
    int heigth1 = (2 * std::max(nin, nout) + 1) * m_diameter * 1.2;
    int heigth2 = m_label->boundingRect().height() + (valueCount + 2) * metrics.height();
    m_heigth = std::max(heigth1, heigth2);

    if (m_heigth > MAX_HEIGTH) {
        m_heigth = MAX_HEIGTH;
    }

    //ширина зависит от максимальной длины подписи или переменной отображения
    int width1 = metrics.size(Qt::TextSingleLine, m_name).width();
    int width2 = m_width;
    m_width = std::max(width1, width2);

    if (m_width > MAX_WIDTH) {
        m_width = MAX_WIDTH;
    }
}

//-------------------------------------------------------------------------------------------------
void GraphicsDevice::configInputs()
{
    m_inputs.clear();
    qreal delta = m_heigth / (m_nin + 1);
    for (int i = 0; i < m_nin; ++i) {
        GraphicsPort* gpoint = new GraphicsPort(this, i, IGraphicsPort::Input);
        gpoint->setPos(m_posx, m_posy + (i + 1) * delta);
        m_inputs.push_back(IGraphicsPortPtr(gpoint));
        gpoint->setParentItem(this);
    }
}

//-------------------------------------------------------------------------------------------------
void GraphicsDevice::configOutputs()
{
    m_outputs.clear();
    qreal delta = m_heigth / (m_nout + 1);
    for (int i = 0; i < m_nout; ++i) {
        GraphicsPort* gpoint = new GraphicsPort(this, i, IGraphicsPort::Output);
        gpoint->setPos(m_posx + m_width, m_posy + (i + 1) * delta);
        m_outputs.push_back(IGraphicsPortPtr(gpoint));
        gpoint->setParentItem(this);
    }
}

//-------------------------------------------------------------------------------------------------
QRectF GraphicsDevice::boundingRect() const
{
    return QRectF(m_posx, m_posy, m_width + SHADOW_WIDTH, m_heigth + SHADOW_WIDTH);
}

//-------------------------------------------------------------------------------------------------
void GraphicsDevice::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    //тень под прямоугольником
    QRectF shadow_rect = QRectF(m_posx + SHADOW_WIDTH, m_posy + SHADOW_WIDTH, m_width, m_heigth);
    painter->setPen(Qt::NoPen);
    painter->setBrush(QColor(0, 0, 0, 40));
    painter->drawRoundedRect(shadow_rect, 5, 5);

    //основной прямоугольник модуля
    QRectF main_rect = QRectF(m_posx, m_posy, m_width, m_heigth);
    painter->setPen(m_border_color);
    painter->setBrush(Qt::white);
    painter->drawRect(main_rect);

    //прямоугольник статуса
    switch (m_status) {
    case IGraphicsDevice::Started:
        painter->setBrush(Qt::green);
        break;

    case IGraphicsDevice::Stopped:
        painter->setBrush(Qt::gray);
        break;

    case IGraphicsDevice::Waiting:
        painter->setBrush(Qt::yellow);
        break;
    }

    painter->drawRect(m_posx, m_posy, m_width / 10, m_heigth);
}

//-------------------------------------------------------------------------------------------------
void GraphicsDevice::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    //отдаем управление стандартному обработчику
    //ЗЫ: важно отдать управление в самом начале,
    //чтобы при команде на удаление не вызвался обработчик для удаленного объекта
    QGraphicsObject::mousePressEvent(event);

    if (event->button() == Qt::LeftButton) {
        //смена курсора
        this->setCursor(QCursor(Qt::ClosedHandCursor));
    } else if (event->button() == Qt::RightButton) {
        //генерация сигнала о нажатии правой кнопки
        QPointF pos = event->scenePos();
        emit rightMouseButtonClicked(pos);
    }
}

//-------------------------------------------------------------------------------------------------
void GraphicsDevice::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
    this->setCursor(QCursor(Qt::OpenHandCursor));

    //отдаем управление стандартному обработчику
    QGraphicsObject::mouseReleaseEvent(event);
}

//-------------------------------------------------------------------------------------------------
void GraphicsDevice::hoverEnterEvent(QGraphicsSceneHoverEvent* event)
{
    this->setCursor(QCursor(Qt::OpenHandCursor));
    m_border_color = Qt::red;
    this->update();

    //отдаем управление стандартному обработчику
    QGraphicsObject::hoverEnterEvent(event);
}

//-------------------------------------------------------------------------------------------------
void GraphicsDevice::hoverLeaveEvent(QGraphicsSceneHoverEvent* event)
{
    this->setCursor(QCursor(Qt::ArrowCursor));
    m_border_color = Qt::black;
    this->update();

    //отдаем управление стандартному обработчику
    QGraphicsObject::hoverLeaveEvent(event);
}

//-------------------------------------------------------------------------------------------------
void GraphicsDevice::mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event)
{
    if (event->button() == Qt::LeftButton) {
        //генерация сигнала о двойном нажатии
        QPointF pos = event->scenePos();
        emit leftMouseDoubleClicked(pos);
    }

    //отдаем управление стандартному обработчику
    QGraphicsObject::mouseDoubleClickEvent(event);
}
