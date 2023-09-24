#pragma once

#include <QDir>
#include <QMap>

#include <memory>

#include <dsplab/i_plugin-factory.h>

/*!
 * \brief Класс загрузчика плагинов
 * \details Обновляет список и формирует в группы плагины из
 * динамических библиотек в данном каталоге.
 */
class PluginLoader
{
public:
    PluginLoader();
    virtual ~PluginLoader();

    //задать директорию для поиска плагинов
    void setPluginDir(QDir dir);

    //получить текущую директорию для поиска плагинов
    QDir getPluginDir() const;

    //получить список плагинов (по группам)
    typedef std::pair<QString, QStringList> PluginGroup;
    QList<PluginGroup> pluginList() const;

    //инстанцировать устройство
    std::shared_ptr<DspLab::IController> instance(quint64 guid);
    std::shared_ptr<DspLab::IController> instance(QString name);

    //поиск по имени и ключу
    QString pluginNameById(quint64 id) const;
    quint64 pluginIdByName(QString name) const;

    //перезагрузка библиотек
    void refresh();

private:
    struct PluginInfo
    {
        quint64 guid;
        QString name;
        QString version;
        QString path;
        DspLab::IPluginFactory* factory;
    };

    struct PluginGroupInfo
    {
        PluginGroupInfo(QString _name = "", QList<PluginInfo> _plugins = {})
          : name(_name)
          , plugins(_plugins)
        {}

        QString name;
        QList<PluginInfo> plugins;
    };

    QMap<QString, PluginInfo> m_plugins;
    QList<PluginGroupInfo> m_groups;
    QDir m_dir;

    //отбор *.dll & *.so по каталогу и загрузка валидных библиотек
    void loadPluginLibs();

    //формирование списка групп
    void formGroupList();

    //инициализация фабрики плагинов
    DspLab::IPluginFactory* instanceFactory(QString path);

    //создать экземпляр плагина
    std::shared_ptr<DspLab::IController> instanceDevice(QString path);
};
