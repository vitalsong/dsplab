#pragma once

#include <QMainWindow>
#include <QJsonDocument>
#include <QMap>

#include <memory>

namespace Ui {
class MainWindow;
}

class IGraphicsPort;
class IGraphicsDevice;
class DeviceFacade;
class PortAdapter;
class MainView;
class MainScene;
class QCpuMonitor;
class PluginLoader;
class PluginLoaderWidget;
class DeviceFacade;
class MenuBar;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = 0);
    ~MainWindow();

private:
    void _configurateMenu();
    void _configurateLogger();
    void _configurateScene();
    void _configuratePluginLoader();
    void _configurateCpuMonitor();
    void _configSceneSignals();
    void _configImportExport();

    //работа с Json
    static QJsonDocument _readJsonDoc(QString name);
    static void _writeJsonDoc(QJsonDocument doc, QString name);

    //поиск по идентификатору граф.объекта
    DeviceFacade* _findDeviceBySceneId(quint32 id);
    PortAdapter* _findPortBySceneId(quint32 id);

    //создание плагина
    void _instacePluginWidget(const QString& name, QPointF pos = QPointF());

    //импорт/экспорт конфигураций
    QJsonDocument _exportConfig();
    void _importConfig(QJsonDocument doc);

private slots:
    void atatchPort(quint32 portId1, quint32 portId2);
    void detachPort(quint32 portId1, quint32 portId2);
    void startAllDevices();
    void stopAllDevices();
    void deleteAllDevices();
    void createSaveDialog();
    void createOpenDialog();
    void saveCurrentConfig();
    void openLastConfig();

private:
    Ui::MainWindow* ui;
    MenuBar* m_menu;
    MainView* m_gview;
    MainScene* m_scene;
    PluginLoader* m_loader;
    QCpuMonitor* m_cpu_monitor;
    PluginLoaderWidget* m_loader_view;

    //список устройств по индексам граф.объектов сцены
    QMap<quint32, std::shared_ptr<DeviceFacade>> m_devs;
};
