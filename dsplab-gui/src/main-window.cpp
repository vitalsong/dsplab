#include "main-window.h"
#include "ui_main-window.h"

#include <sys-monitor/qcpu-monitor.h>
#include <loading/plugin-loader.h>
#include <loading/plugin-loader-widget.h>
#include <scene/main-view.h>
#include <scene/main-scene.h>
#include <logging/logger.h>
#include <menu/menu-bar.h>
#include <device-config.h>

#include <QGraphicsView>
#include <QSplitter>
#include <QJsonArray>
#include <QLabel>
#include <QTimer>
#include <QMessageBox>
#include <QFileDialog>

#include <scene/i_graphics-device.h>
#include <scene/i_graphics-port.h>
#include <base/device-facade.h>
#include <base/port-adapter.h>

//-------------------------------------------------------------------------------------------------
MainWindow::MainWindow(QWidget* parent)
  : QMainWindow(parent)
  , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //предварительно конфигурируем все модули
    _configurateLogger();
    _configuratePluginLoader();
    _configurateMenu();
    _configurateScene();
    _configurateCpuMonitor();
    _configImportExport();

    //настройка сплиттеров
    ui->splSceneAndLoader->setSizes({256, 2048});
    ui->splMainAndLogger->setSizes({2048, 128});

    //диалоговое окно запроса на загрузку прошлой конфигурации (с задержкой в 2 сек)
    QTimer::singleShot(2000, this, &MainWindow::openLastConfig);
}

//-------------------------------------------------------------------------------------------------
MainWindow::~MainWindow()
{
    //сохраняем конфигурацию по-умолчанию
    QJsonDocument jdoc = _exportConfig();
    _writeJsonDoc(jdoc, "lconfig.ini");

    //очищаем список устройств
    m_devs.clear();

    //удаляем объект фабрики устройств
    delete m_loader;

    delete ui;
}

//-------------------------------------------------------------------------------------------------
void MainWindow::_configurateMenu()
{
    //создаем меню приложения
    m_menu = new MenuBar(this);
    QMainWindow::setMenuBar(m_menu);

    //реакция на команды запуска/остановки всех устройств
    connect(m_menu, &MenuBar::startAllChecked, this, &MainWindow::startAllDevices);

    connect(m_menu, &MenuBar::stopAllChecked, this, &MainWindow::stopAllDevices);

    //удаление всех элементов
    connect(m_menu, &MenuBar::deleteAllChecked, this, &MainWindow::deleteAllDevices);
}

//-------------------------------------------------------------------------------------------------
void MainWindow::_configurateLogger()
{
    QListWidget* widget = new QListWidget();
    ui->loggerLayout->addWidget(widget);
    Logger::instance(widget);
}

//-------------------------------------------------------------------------------------------------
void MainWindow::_configurateScene()
{
    m_scene = new MainScene();
    m_gview = new MainView(this);
    ui->sceneLayout->addWidget(m_gview);
    m_gview->setScene(m_scene);
    m_gview->setRenderHint(QPainter::Antialiasing);

    //запуск устройства
    connect(m_scene, &MainScene::startDevice, [&](quint32 id) {
        _findDeviceBySceneId(id)->start();
    });

    //остановка устройства
    connect(m_scene, &MainScene::stopDevice, [&](quint32 id) {
        _findDeviceBySceneId(id)->stop();
    });

    //показать виджет устройства
    connect(m_scene, &MainScene::showDevice, [&](quint32 id) {
        _findDeviceBySceneId(id)->show();
    });

    //удаление устройства
    connect(m_scene, &MainScene::deleteDevice, [&](quint32 id) {
        m_devs.remove(id);
    });

    //инстанцировать устройство по сигналу от сцены (при переносе drug and drop)
    connect(m_gview, &MainView::createDragDropDevice, [&](QString name, QPointF pos) {
        _instacePluginWidget(name, pos);
    });

    connect(m_scene, &MainScene::attachPort, this, &MainWindow::atatchPort);

    connect(m_scene, &MainScene::detachPort, this, &MainWindow::detachPort);
}

//-------------------------------------------------------------------------------------------------
void MainWindow::_configuratePluginLoader()
{
    m_loader = new PluginLoader();
    const auto pluginDir = QApplication::applicationDirPath() + "/plugins";
    m_loader->setPluginDir(pluginDir);
    m_loader->refresh();

    //виджет со списком плагинов
    m_loader_view = new PluginLoaderWidget();
    m_loader_view->setPluginList(m_loader->pluginList());
    ui->loaderLayout->addWidget(m_loader_view);

    //инстанцирование плагина
    connect(m_loader_view, &PluginLoaderWidget::pluginDoubleClicked, [&](QString name) {
        _instacePluginWidget(name);
    });
}

//-------------------------------------------------------------------------------------------------
void MainWindow::_configurateCpuMonitor()
{
#ifdef __WIN32__
    //монитор загрузки процессора
    m_cpu_monitor = new QCpuMonitor();
    m_cpu_monitor->setInterval(1);
    m_cpu_monitor->start();

    //настройка строки состояния
    QLabel* label = new QLabel();
    ui->statusBar->addWidget(label);
    connect(m_cpu_monitor, &QCpuMonitor::textChanged, label, &QLabel::setText);
#endif
}

//-------------------------------------------------------------------------------------------------
void MainWindow::_configImportExport()
{
    connect(m_menu, &MenuBar::saveAsChecked, this, &MainWindow::createSaveDialog);

    connect(m_menu, &MenuBar::openChecked, this, &MainWindow::createOpenDialog);

    connect(m_menu, &MenuBar::saveChecked, this, &MainWindow::saveCurrentConfig);
}

//-------------------------------------------------------------------------------------------------
QJsonDocument MainWindow::_readJsonDoc(QString name)
{
    QFile file(name);
    if (!file.open(QIODevice::ReadOnly)) {
        return QJsonDocument();
    }

    QByteArray data = file.readAll();
    QJsonDocument doc = QJsonDocument::fromBinaryData(data);
    return doc;
}

//-------------------------------------------------------------------------------------------------
void MainWindow::_writeJsonDoc(QJsonDocument doc, QString name)
{
    QFile file(name);
    if (!file.open(QIODevice::WriteOnly)) {
        return;
    }

    QByteArray data = doc.toBinaryData();
    file.write(data);
}

//-------------------------------------------------------------------------------------------------
void MainWindow::_instacePluginWidget(const QString& name, QPointF pos)
{
    //создаем объект устройства по имени
    auto dev = m_loader->instance(name);

    if (dev.get() != nullptr) {
        //создаем экземпляр на сцене
        auto view = m_scene->instance(pos);

        //обеспечение модальности окна
        if (dev->window() != nullptr) {
            dev->window()->setParent(m_gview, Qt::Dialog);
        }

        //конструируем фасад объекта
        DeviceFacade* facade = new DeviceFacade(dev, view, name);

        //сохраняем в списке устройств
        m_devs.insert(view->id(), std::shared_ptr<DeviceFacade>(facade));
    }
}

//-------------------------------------------------------------------------------------------------
QJsonDocument MainWindow::_exportConfig()
{
    QJsonArray jarray;

    //проходим по списку всех устройств
    for (auto dev : m_devs) {
        DeviceConfig config;
        config.setDeviceId(dev->id());
        config.setPluginId(m_loader->pluginIdByName(dev->name()));

        //входные соединения
        DeviceConfig::ConnectionList list;
        int count = dev->nin();
        for (int i = 0; i < count; ++i) {
            PortAdapter* p1 = dev->input(i);
            auto connections = p1->connections();
            for (auto p2 : connections) {
                list.push_back({p2->parent()->id(), quint32(p2->num()), quint32(i)});
            }
        }

        config.setConnections(list);

        //список параметров
        config.setParams(dev->paramList());

        //координаты на сцене
        config.setScenePos(dev->sceneView()->getScenePos());

        //положение виджета
        QWidget* dialog = dev->controller()->window();
        if (dialog != nullptr) {
            config.setWidgetVisible(dialog->isVisible());
            config.setWidgetPos(dialog->pos());
            config.setWidgetSize(dialog->size());
        }

        //генерируем map
        QVariantMap map = DeviceConfig::toVariantMap(config);

        //добавляем в список
        jarray.push_back(QJsonValue::fromVariant(QVariant(map)));
    }

    //парсинг в документ
    QJsonDocument jdoc(jarray);

    return jdoc;
}

//-------------------------------------------------------------------------------------------------
void MainWindow::_importConfig(QJsonDocument doc)
{
    //сначала необходимо остановить и удалить все устройства
    stopAllDevices();
    deleteAllDevices();

    const QJsonArray jarray = doc.array();
    QVector<DeviceConfig> configList;
    QMap<quint32, quint32> keys;

    //конструирование объектов-фасадов
    for (auto it : jarray) {
        QVariantMap map = it.toVariant().toMap();
        DeviceConfig config = DeviceConfig::fromVariantMap(map);
        configList.push_back(config);

        //создание экземпляра по идентификатору библиотеки
        auto dev = m_loader->instance(config.pluginId());

        //обеспечение модальности окна
        auto dialog = dev->window();
        if (dialog != nullptr) {
            dialog->setParent(this, Qt::Dialog);
            dialog->resize(config.widgetSize());
            dialog->move(config.widgetPos());

            if (config.widgetVisible()) {
                dialog->show();
            }
        }

        //создание графического отображения
        auto view = m_scene->instance();
        view->setScenePos(config.scenePos());

        //конструирование объекта фасада
        QString name = m_loader->pluginNameById(config.pluginId());
        DeviceFacade* facade = new DeviceFacade(dev, view, name);

        //список соответствия старых идентификаторов к новым
        keys[config.deviceId()] = facade->id();

        //настройка параметров
        facade->setParam(config.params());

        //добавляем объект в список
        m_devs[view->id()] = std::shared_ptr<DeviceFacade>(facade);
    }

    //настройка коммутации
    for (DeviceConfig& config : configList) {
        //поиск первого устройства
        DeviceFacade* dev1;
        for (auto dev : m_devs) {
            if (dev->id() == keys[config.deviceId()]) {
                dev1 = dev.get();
                break;
            }
        }

        //обработка всех подключений
        auto connections = config.connections();
        for (auto cn : connections) {
            for (auto dev : m_devs) {
                if (dev->id() == keys[cn.devId]) {
                    DeviceFacade* dev2 = dev.get();
                    PortAdapter* p1 = dev1->input(cn.selfPort);
                    PortAdapter* p2 = dev2->output(cn.devPort);
                    p1->attach(p2);
                    break;
                }
            }
        }
    }
}

//-------------------------------------------------------------------------------------------------
DeviceFacade* MainWindow::_findDeviceBySceneId(quint32 id)
{
    //поиск портов
    for (auto dev : m_devs) {
        if (dev->sceneView()->id() == id) {
            return dev.get();
        }
    }

    return nullptr;
}

//-------------------------------------------------------------------------------------------------
PortAdapter* MainWindow::_findPortBySceneId(quint32 id)
{
    //поиск портов
    for (auto dev : m_devs) {
        auto view = dev->sceneView();
        IGraphicsPort* viewPort;

        //поиск по входным
        viewPort = view->input(id);
        if (viewPort != nullptr) {
            return dev->input(viewPort->num());
        }

        //поиск по выходным
        viewPort = view->output(id);
        if (viewPort != nullptr) {
            return dev->output(viewPort->num());
        }
    }

    return nullptr;
}

//-------------------------------------------------------------------------------------------------
void MainWindow::atatchPort(quint32 portId1, quint32 portId2)
{
    //поиск портов
    PortAdapter* port1 = _findPortBySceneId(portId1);
    PortAdapter* port2 = _findPortBySceneId(portId2);

    if (!port1 || !port2) {
        Logger::print(Logger::Error, "Ошибка коммутации. Не найден порт по идентификатору.");
        return;
    }

    //коммутация
    port1->attach(port2);
}

//-------------------------------------------------------------------------------------------------
void MainWindow::detachPort(quint32 portId1, quint32 portId2)
{
    //поиск портов
    PortAdapter* port1 = _findPortBySceneId(portId1);
    PortAdapter* port2 = _findPortBySceneId(portId2);

    if (!port1 || !port2) {
        Logger::print(Logger::Error, "Ошибка коммутации. Не найден порт по идентификатору.");
        return;
    }

    //коммутация
    port1->detach(port2);
}

//-------------------------------------------------------------------------------------------------
void MainWindow::startAllDevices()
{
    for (auto dev : m_devs) {
        dev->start();
    }
}

//-------------------------------------------------------------------------------------------------
void MainWindow::stopAllDevices()
{
    for (auto dev : m_devs) {
        dev->stop();
    }
}

//-------------------------------------------------------------------------------------------------
void MainWindow::deleteAllDevices()
{
    stopAllDevices();
    m_devs.clear();
}

//-------------------------------------------------------------------------------------------------
void MainWindow::createSaveDialog()
{
    QJsonDocument jdoc = _exportConfig();
    QString name = QApplication::applicationDirPath() + QString("/presets/default.ini");
    name = QFileDialog::getSaveFileName(this, tr("Сохранить конфигурацию"), name);
    Logger::print(Logger::Info, tr("Импорт конфигурации в файл: ") + name);
    _writeJsonDoc(jdoc, name);
}

//-------------------------------------------------------------------------------------------------
void MainWindow::createOpenDialog()
{
    QString name = QApplication::applicationDirPath() + QString("/presets/default.ini");
    name = QFileDialog::getOpenFileName(this, tr("Открыть конфигурацию"), name);
    Logger::print(Logger::Info, tr("Экспорт конфигурации из файла: ") + name);
    QJsonDocument jdoc = _readJsonDoc(name);
    _importConfig(jdoc);
}

//-------------------------------------------------------------------------------------------------
void MainWindow::saveCurrentConfig()
{
    QJsonDocument jdoc = _exportConfig();
    QString name = QApplication::applicationDirPath() + QString("/lconfig.ini");
    _writeJsonDoc(jdoc, name);
}

//-------------------------------------------------------------------------------------------------
void MainWindow::openLastConfig()
{
    QString name = "lconfig.ini";

    if (!QFile::exists(name)) {
        return;
    }

    QMessageBox msgBox;
    msgBox.setText("Обнаружена прошлая конфигурация. Загрузить?");
    msgBox.setStandardButtons(QMessageBox::Open | QMessageBox::Close);
    msgBox.setDefaultButton(QMessageBox::Open);
    int ret = msgBox.exec();

    if (ret == QMessageBox::Open) {
        Logger::print(Logger::Info, tr("Экспорт конфигурации из файла: ") + name);
        QJsonDocument doc = _readJsonDoc(name);
        _importConfig(doc);
    }
}
