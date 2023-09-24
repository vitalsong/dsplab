#pragma once

#include <QVariant>
#include <QPointF>
#include <QSizeF>
#include <QSettings>

class DeviceFacade;

/*!
 * \brief Описание полной конфигурации устройства для дальнейшего экспорта/импорта
 * \todo Имеет смысл перейти от класса к простой структуре
 */
class DeviceConfig
{
public:
    DeviceConfig();

    struct Connection
    {
        quint32 devId;   ///< идентификатор устройства с которым имеется подключение
        quint32 devPort;    ///< номер порта устройства с подключением
        quint32 selfPort;   ///< номер порта устройства родителя
    };

    //список соединений для одного порта
    typedef std::vector<Connection> ConnectionList;

    //получение параметров
    quint32 deviceId() const;
    quint32 pluginId() const;
    quint32 pluginVersion() const;
    const ConnectionList& connections() const;
    QVariantMap params() const;
    QPointF scenePos() const;

    bool widgetVisible() const;
    QPoint widgetPos() const;
    QSize widgetSize() const;

    void setWidgetVisible(bool visible);
    void setWidgetPos(QPoint pos);
    void setWidgetSize(QSize size);

    //настройка параметров
    void setDeviceId(quint32 id);
    void setPluginId(quint32 id);
    void setPluginVersion(quint32 ver);
    void setConnections(ConnectionList connections);
    void setParams(QVariantMap params);
    void setScenePos(QPointF pos);

    //экспорт/импорт конфигурации
    static DeviceConfig fromVariantMap(QVariantMap map);
    static QVariantMap toVariantMap(DeviceConfig config);

private:
    quint32 m_deviceId;
    quint32 m_pluginId;
    ConnectionList m_connections;
    QVariantMap m_params;
    QPointF m_pos;
    QPoint m_widget_pos;
    QSize m_widget_size;
    bool m_widget_visible;
};
