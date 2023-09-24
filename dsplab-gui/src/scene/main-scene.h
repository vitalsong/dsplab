#pragma once

#include <QGraphicsScene>

#include <memory>

class IGraphicsDevice;
class IGraphicsPort;
class GraphicsPort;

/*!
 * \brief Базовая сцена для размещения всех элементов
 */
class MainScene : public QGraphicsScene
{
    Q_OBJECT

public:
    explicit MainScene(QObject* parent = 0);
    virtual ~MainScene();

    //размещение базового графического объекта
    std::shared_ptr<IGraphicsDevice> instance(QPointF pos = QPointF());

signals:

    //запрос на коммутацию со стороны UI
    void attachPort(quint32 portId1, quint32 portId2);
    void detachPort(quint32 portId1, quint32 portId2);

    //запрос на запуск/остановку со стороны UI
    void startDevice(quint32 devId);
    void stopDevice(quint32 devId);

    //запрос на отображение со стороны UI
    void showDevice(quint32 devId);

    //запрос на удаление объекта со стороны UI
    void deleteDevice(quint32 devId);

private:
    //------------------------------------------------
    //параметры временной линии для соединения портов
    //------------------------------------------------

    //обновить соединение временной линии
    void tempLineFirstConnect(GraphicsPort* port);
    void tempLineSecondConnect(GraphicsPort* port);

    //обновить позицию временной линии (по курсору мыши)
    void tempLineCursorMoving(QPointF pos);

    //очистить параметры временной линии
    void tempLineClear();

    //разрыв всех соединений на устройстве
    void detachAllConnections(IGraphicsDevice* dev);

    //------------------------------------------------
    //перегрузка виртуальных функций сцены
    //------------------------------------------------

    //события удержания/отпускания клавиши
    void mousePressEvent(QGraphicsSceneMouseEvent* event) final;

    //событие движения мыши
    void mouseMoveEvent(QGraphicsSceneMouseEvent* event) final;

private slots:
    void createContextMenu(QPointF pos);

private:
    //временные параметры для соединения
    bool m_wait_connect;
    GraphicsPort* m_temp_port;
    QGraphicsLineItem* m_temp_line;
};
