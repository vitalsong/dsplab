#include <loading/plugin-loader.h>
#include <logging/logger.h>

#include <QLibrary>
#include <QDirIterator>

//-------------------------------------------------------------------------------------------------
PluginLoader::PluginLoader()
{
    m_dir.setPath("./");
}

//-------------------------------------------------------------------------------------------------
PluginLoader::~PluginLoader()
{
    //nothing to do
}

//-------------------------------------------------------------------------------------------------
void PluginLoader::setPluginDir(QDir dir)
{
    m_dir = dir;
    Logger::print(Logger::Info, QString("Plugins directory: ") + dir.absolutePath());
}

//-------------------------------------------------------------------------------------------------
QDir PluginLoader::getPluginDir() const
{
    return m_dir;
}

//-------------------------------------------------------------------------------------------------
QList<PluginLoader::PluginGroup> PluginLoader::pluginList() const
{
    QList<PluginLoader::PluginGroup> list;

    for (auto groupInfo : m_groups) {
        PluginLoader::PluginGroup group;
        group.first = groupInfo.name;
        for (auto pluginInfo : groupInfo.plugins) {
            group.second.push_back(pluginInfo.name);
        }

        list.push_back(group);
    }

    return list;
}

//-------------------------------------------------------------------------------------------------
std::shared_ptr<DspLab::IController> PluginLoader::instance(quint64 guid)
{
    //поиск плагина по имени
    for (auto groupInfo : m_groups) {
        for (auto pluginInfo : groupInfo.plugins) {
            if (pluginInfo.guid == guid) {
                return this->instanceDevice(pluginInfo.path);
            }
        }
    }

    //сообщение об ошибке
    //...

    return nullptr;
}

//-------------------------------------------------------------------------------------------------
std::shared_ptr<DspLab::IController> PluginLoader::instance(QString name)
{
    //поиск плагина по имени
    for (auto groupInfo : m_groups) {
        for (auto pluginInfo : groupInfo.plugins) {
            if (pluginInfo.name == name) {
                return this->instanceDevice(pluginInfo.path);
            }
        }
    }

    //сообщение об ошибке
    //...

    return nullptr;
}

//-------------------------------------------------------------------------------------------------
QString PluginLoader::pluginNameById(quint64 id) const
{
    for (auto groupInfo : m_groups) {
        for (auto pluginInfo : groupInfo.plugins) {
            if (pluginInfo.guid == id) {
                return pluginInfo.name;
            }
        }
    }

    return "";
}

//-------------------------------------------------------------------------------------------------
quint64 PluginLoader::pluginIdByName(QString name) const
{
    for (auto groupInfo : m_groups) {
        for (auto pluginInfo : groupInfo.plugins) {
            if (pluginInfo.name == name) {
                return pluginInfo.guid;
            }
        }
    }

    return 0;
}

//-------------------------------------------------------------------------------------------------
void PluginLoader::refresh()
{
    Logger::print(Logger::Info, "Update plugins list");

    //загрузка библиотек плагинов из текущего каталога
    this->loadPluginLibs();

    //формирование списка групп (по отфильтрованным плагинам)
    this->formGroupList();
}

//-------------------------------------------------------------------------------------------------
static QStringList sEntryListRecursive(QDir dir)
{
    QStringList list;
    QDirIterator it(dir, QDirIterator::Subdirectories);
    while (it.hasNext()) {
        if (it.fileInfo().absoluteFilePath() == it.next()) {
            continue;
        }

        QDir dir(it.filePath());
        QStringList entry = dir.entryList(QStringList({"*.dll", "*.so", "*.dylib"}), QDir::Files);

        for (QString& s : entry) {
            s = dir.absoluteFilePath(s);
        }

        list += entry;
    }

    return list;
}

//-------------------------------------------------------------------------------------------------
void PluginLoader::loadPluginLibs()
{
    QStringList libList = sEntryListRecursive(m_dir);
    QLibrary lib;

    for (const QString& file : libList) {
        //установим имя библиотеки
        lib.setFileName(file);

        //если библиотека не загрузилась
        if (!lib.load()) {
            Logger::print(Logger::Warning, lib.errorString());
            continue;
        }

        //инстанцирование фабрики
        typedef DspLab::IPluginFactory* (*PLUGIN_INSTANCE_PROTOTYPE)(void);
        PLUGIN_INSTANCE_PROTOTYPE fnInstance = (PLUGIN_INSTANCE_PROTOTYPE)lib.resolve("instance");

        //имя плагина
        typedef const char* (*PLUGIN_NAME_PROTOTYPE)(void);
        PLUGIN_NAME_PROTOTYPE fnName = (PLUGIN_NAME_PROTOTYPE)lib.resolve("name");

        //уникальный идентификатор плагина
        typedef uint64_t (*PLUGIN_GUID_PROTOTYPE)(void);
        PLUGIN_GUID_PROTOTYPE fnGuid = (PLUGIN_GUID_PROTOTYPE)lib.resolve("guid");

        //версия плагина
        typedef const char* (*PLUGIN_VERSION_PROTOTYPE)(void);
        PLUGIN_VERSION_PROTOTYPE fnVersion = (PLUGIN_VERSION_PROTOTYPE)lib.resolve("version");

        //если все функции были выгружены
        if (fnInstance && fnName && fnGuid && fnVersion) {
            //обновление списка
            PluginInfo info = {fnGuid(), fnName(), fnVersion(), file, nullptr};
            m_plugins.insert(file, info);
        } else {
            Logger::print(Logger::Error, QString("Library API is not valid: ") + lib.fileName());
        }
    }
}

//-------------------------------------------------------------------------------------------------
void PluginLoader::formGroupList()
{
    m_groups.clear();

    QStringList listCatalog;
    for (auto it = m_plugins.begin(); it != m_plugins.end(); ++it) {
        QString path = it.key();
        size_t index = path.lastIndexOf("/");
        path = path.remove(index, path.size() - index);

        QStringList::iterator iterPath = qFind(listCatalog.begin(), listCatalog.end(), path);
        if (iterPath == listCatalog.end()) {
            QList<PluginInfo> plugins;
            for (auto it = m_plugins.begin(); it != m_plugins.end(); ++it) {
                QString pathPlugin = it.key();
                size_t indexPlugin = pathPlugin.lastIndexOf("/");
                pathPlugin = pathPlugin.remove(indexPlugin, pathPlugin.size() - indexPlugin);

                if (path == pathPlugin) {
                    plugins.push_back(it.value());
                }
            }

            QString group(path);
            group.remove(0, group.lastIndexOf("/") + 1);

            PluginGroupInfo paramGroup(group, plugins);
            m_groups.push_back(paramGroup);
            listCatalog.push_back(path);
        }
    }
}

//-------------------------------------------------------------------------------------------------
DspLab::IPluginFactory* PluginLoader::instanceFactory(QString path)
{
    QLibrary lib(path);
    typedef DspLab::IPluginFactory* (*FUNC_INSTANCE)();
    FUNC_INSTANCE fnInstance = (FUNC_INSTANCE)lib.resolve("instance");

    if (fnInstance != nullptr) {
        return fnInstance();
    }

    return nullptr;
}

//-------------------------------------------------------------------------------------------------
std::shared_ptr<DspLab::IController> PluginLoader::instanceDevice(QString path)
{
    //инстанцируем фабрику
    DspLab::IPluginFactory* factory = this->instanceFactory(path);

    //создаем объект
    return factory->create();
}
