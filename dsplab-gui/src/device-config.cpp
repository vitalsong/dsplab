#include "device-config.h"

//-------------------------------------------------------------------------------------------------
DeviceConfig::DeviceConfig()
  : m_deviceId(0)
  , m_pluginId(0)
{
    //nothing to do
}

//-------------------------------------------------------------------------------------------------
quint32 DeviceConfig::deviceId() const
{
    return m_deviceId;
}

//-------------------------------------------------------------------------------------------------
quint32 DeviceConfig::pluginId() const
{
    return m_pluginId;
}

//-------------------------------------------------------------------------------------------------
quint32 DeviceConfig::pluginVersion() const
{
    return 1;
}

//-------------------------------------------------------------------------------------------------
const DeviceConfig::ConnectionList& DeviceConfig::connections() const
{
    return m_connections;
}

//-------------------------------------------------------------------------------------------------
QVariantMap DeviceConfig::params() const
{
    return m_params;
}

//-------------------------------------------------------------------------------------------------
QPointF DeviceConfig::scenePos() const
{
    return m_pos;
}

//-------------------------------------------------------------------------------------------------
bool DeviceConfig::widgetVisible() const
{
    return m_widget_visible;
}

//-------------------------------------------------------------------------------------------------
QPoint DeviceConfig::widgetPos() const
{
    return m_widget_pos;
}

//-------------------------------------------------------------------------------------------------
QSize DeviceConfig::widgetSize() const
{
    return m_widget_size;
}

//-------------------------------------------------------------------------------------------------
void DeviceConfig::setDeviceId(quint32 id)
{
    m_deviceId = id;
}

//-------------------------------------------------------------------------------------------------
void DeviceConfig::setPluginId(quint32 id)
{
    m_pluginId = id;
}

//-------------------------------------------------------------------------------------------------
void DeviceConfig::setPluginVersion(quint32 ver)
{
    Q_UNUSED(ver);
}

//-------------------------------------------------------------------------------------------------
void DeviceConfig::setConnections(ConnectionList connections)
{
    m_connections = connections;
}

//-------------------------------------------------------------------------------------------------
void DeviceConfig::setParams(QVariantMap params)
{
    m_params = params;
}

//-------------------------------------------------------------------------------------------------
void DeviceConfig::setScenePos(QPointF pos)
{
    m_pos = pos;
}

//-------------------------------------------------------------------------------------------------
void DeviceConfig::setWidgetVisible(bool visible)
{
    m_widget_visible = visible;
}

//-------------------------------------------------------------------------------------------------
void DeviceConfig::setWidgetPos(QPoint pos)
{
    m_widget_pos = pos;
}

//-------------------------------------------------------------------------------------------------
void DeviceConfig::setWidgetSize(QSize size)
{
    m_widget_size = size;
}

//-------------------------------------------------------------------------------------------------
DeviceConfig DeviceConfig::fromVariantMap(QVariantMap map)
{
    DeviceConfig config;

    //идентификация
    config.setDeviceId(map["device_id"].toUInt());
    config.setPluginId(map["plugin_id"].toUInt());

    //параметры коммутации
    ConnectionList cList;
    int count = map["connections_count"].toInt();
    for (int i = 0; i < count; ++i) {
        QString key = QString("connection_") + QString::number(i);
        Connection c = {map[key].toList().at(0).toUInt(), map[key].toList().at(1).toUInt(),
                        map[key].toList().at(2).toUInt()};

        cList.push_back(c);
    }

    config.setConnections(cList);

    //параметры устройства
    config.setParams(map["params"].toMap());

    //координаты на сцене
    QPointF scene_pos;
    scene_pos.setX(map["scene_pos_x"].toReal());
    scene_pos.setY(map["scene_pos_y"].toReal());
    config.setScenePos(scene_pos);

    //координаты виджета
    QPoint widget_pos = {map["widget_pos_x"].toInt(), map["widget_pos_y"].toInt()};
    config.setWidgetPos(widget_pos);

    //видимость виджета
    config.setWidgetVisible(map["widget_visible"].toBool());

    //размер виджета
    QSize widget_size = {map["widget_width"].toInt(), map["widget_height"].toInt()};
    config.setWidgetSize(widget_size);

    return config;
}

//-------------------------------------------------------------------------------------------------
QVariantMap DeviceConfig::toVariantMap(DeviceConfig config)
{
    QVariantMap map;

    //идентификация
    map.insert("device_id", config.deviceId());
    map.insert("plugin_id", config.pluginId());

    //параметры коммутации
    const ConnectionList& cList = config.connections();
    int count = cList.size();
    map.insert("connections_count", count);
    for (int i = 0; i < count; ++i) {
        QString key = QString("connection_") + QString::number(i);
        map.insert(key, QVariantList({cList[i].devId, cList[i].devPort, cList[i].selfPort}));
    }

    //параметры устройства
    map.insert("params", config.params());

    //координаты на сцене
    map.insert("scene_pos_x", QVariant(config.scenePos().x()));
    map.insert("scene_pos_y", QVariant(config.scenePos().y()));

    //координаты виджета
    map.insert("widget_pos_x", QVariant(config.widgetPos().x()));
    map.insert("widget_pos_y", QVariant(config.widgetPos().y()));

    //видимость виджета
    map.insert("widget_visible", QVariant(config.widgetVisible()));

    //размер виджета
    map.insert("widget_width", QVariant(config.widgetSize().width()));
    map.insert("widget_height", QVariant(config.widgetSize().height()));

    return map;
}
