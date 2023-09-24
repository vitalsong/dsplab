#include <scene/main-scene.h>
#include <scene/graphics-device.h>
#include <scene/graphics-port.h>

#include <QGraphicsSceneEvent>
#include <QMenu>

//-------------------------------------------------------------------------------------------------
MainScene::MainScene(QObject* parent)
  : QGraphicsScene(parent)
  , m_wait_connect(false)
  , m_temp_line(nullptr)
{
    //размер сцены
    QGraphicsScene::setSceneRect(-250, -250, 500, 500);

    //индексация элементов
    QGraphicsScene::setItemIndexMethod(QGraphicsScene::NoIndex);
}

//-------------------------------------------------------------------------------------------------
MainScene::~MainScene()
{
    //nothing to do
}

//-------------------------------------------------------------------------------------------------
std::shared_ptr<IGraphicsDevice> MainScene::instance(QPointF pos)
{
    //конфигурируем отображение устройства
    GraphicsDevice* item = new GraphicsDevice();

    //следим за нажатием правой кнопки (вывод меню)
    connect(item, &GraphicsDevice::rightMouseButtonClicked, this, &MainScene::createContextMenu);

    //следим за двойным нажатием левой кнопки (отображение формы)
    connect(item, &GraphicsDevice::leftMouseDoubleClicked, [item, this]() {
        emit showDevice(item->id());
    });

    //добавляем на сцену
    QGraphicsScene::addItem(item);

    //располагаем на верхнем слое сцены
    item->setZValue(1);

    //если координаты не заданы, то распологаем в случайной позиции
    if (pos == QPointF()) {
        //располагаем в случайном месте
        item->setPos(rand() % 100, rand() % 100);
    } else {
        item->setPos(pos);
    }

    return std::shared_ptr<IGraphicsDevice>(item);
}

//-------------------------------------------------------------------------------------------------
void MainScene::tempLineFirstConnect(GraphicsPort* port)
{
    //создаем экземпляр линии для предварительного рисования
    m_temp_line = new QGraphicsLineItem();

    qreal x1 = port->scenePos().x();
    qreal y1 = port->scenePos().y();
    qreal x2 = port->scenePos().x();
    qreal y2 = port->scenePos().y();
    m_temp_line->setLine(x1, y1, x2, y2);

    //высший приоритет отображения
    m_temp_line->setZValue(2);

    //добавляем линию на сцену
    QGraphicsScene::addItem(m_temp_line);

    //сохраняем объект порта для дальнейшей коммутации
    m_temp_port = port;

    //выставляем фиксированную подсветку
    m_temp_port->setSwitched(true);
    m_temp_port->setFixedSwitched(true);

    //выставляем статус ожидания соединения
    m_wait_connect = true;
}

//-------------------------------------------------------------------------------------------------
void MainScene::tempLineSecondConnect(GraphicsPort* port)
{
    //генерируем сообщение о подключении
    emit attachPort(port->id(), m_temp_port->id());

    //удаляем временную линию
    this->tempLineClear();

    //обновляем сцену
    QGraphicsScene::update();
}

//-------------------------------------------------------------------------------------------------
void MainScene::tempLineCursorMoving(QPointF pos)
{
    //координаты линии
    QLineF p0 = m_temp_line->line();

    //меняем координаты второй точки линии
    m_temp_line->setLine(p0.x1(), p0.y1(), pos.x(), pos.y());

    //обновляем сцену
    QGraphicsScene::update();
}

//-------------------------------------------------------------------------------------------------
void MainScene::tempLineClear()
{
    if (m_temp_line != nullptr) {
        //удаляем объект линии
        delete m_temp_line;
        m_temp_line = nullptr;

        //отключаем подсветку временного порта
        m_temp_port->setFixedSwitched(false);
        m_temp_port->setSwitched(false);

        //сбрасываем статус ожидания соединения
        m_wait_connect = false;
    }

    //обновляем сцену
    QGraphicsScene::update();
}

//-------------------------------------------------------------------------------------------------
void MainScene::detachAllConnections(IGraphicsDevice* dev)
{
    //генерация запросов на разрыв соединения всех входных портов
    int nin = dev->getNumInputs();
    for (int i = 0; i < nin; ++i) {
        auto p1 = dev->input(i);
        auto list = p1->connections();
        for (auto p2 : list) {
            emit detachPort(p1->id(), p2->id());
        }
    }

    //генерация запросов на разрыв соединения всех выходных портов
    int nout = dev->getNumOutputs();
    for (int i = 0; i < nout; ++i) {
        auto p1 = dev->output(i);
        auto list = p1->connections();
        for (auto p2 : list) {
            emit detachPort(p1->id(), p2->id());
        }
    }
}

//-------------------------------------------------------------------------------------------------
void MainScene::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    QPointF pos = event->scenePos();
    QList<QGraphicsItem*> list = this->items(pos, Qt::IntersectsItemBoundingRect, Qt::AscendingOrder);

    if (event->button() == Qt::LeftButton) {
        for (QGraphicsItem* it : list) {
            //если имело место нажатие на объект порта
            GraphicsPort* port = dynamic_cast<GraphicsPort*>(it);
            if (port != nullptr) {
                //если временная линия не ожидает соединения со вторым портом
                if (!m_wait_connect) {
                    this->tempLineFirstConnect(port);
                } else {
                    this->tempLineSecondConnect(port);
                }

                //выходим, чтобы объект устройства не получил управление
                return;
            }
        }
    }

    //удаляем линию, если имело место нажатие в другую область
    this->tempLineClear();

    //отдаем управление стандартному обработчику
    QGraphicsScene::mousePressEvent(event);
}

//-------------------------------------------------------------------------------------------------
void MainScene::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
    //вызов функции по-умолчанию
    QGraphicsScene::mouseMoveEvent(event);

    //если временная линия существует
    if (m_wait_connect) {
        this->tempLineCursorMoving(event->scenePos());
    }
}

//-------------------------------------------------------------------------------------------------
void MainScene::createContextMenu(QPointF pos)
{
    Q_UNUSED(pos);

    GraphicsDevice* dev = qobject_cast<GraphicsDevice*>(QObject::sender());

    QMenu menu;
    QAction* start_action = menu.addAction("Включить");
    QAction* stop_action = menu.addAction("Выключить");
    QAction* detach_action = menu.addAction("Разорвать");
    QAction* delete_action = menu.addAction("Удалить");

    //настройка меню для событий вкл/выкл
    start_action->setCheckable(true);
    stop_action->setCheckable(true);

    bool started = dev->getStartedStatus() == IGraphicsDevice::Started;
    start_action->setChecked(started);
    stop_action->setChecked(!started);

    //текущее выбранное действие
    QAction* current_action = menu.exec(QCursor::pos());

    //обработка выбора меню
    if (current_action == start_action) {
        emit startDevice(dev->id());
    } else if (current_action == stop_action) {
        emit stopDevice(dev->id());
    } else if (current_action == detach_action) {
        this->detachAllConnections(dev);
    } else if (current_action == delete_action) {
        emit deleteDevice(dev->id());
    }
}
