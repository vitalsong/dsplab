#pragma once

#include <QWidget>

namespace Ui {
class PluginLoader;
}

class PluginTreeWidget;
class PluginFindWidget;

/*!
 * \brief Виджет со списком плагинов
 */
class PluginLoaderWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PluginLoaderWidget(QWidget* parent = nullptr);
    virtual ~PluginLoaderWidget();

    typedef std::pair<QString, QStringList> PluginGroup;
    void setPluginList(QList<PluginGroup> list);
    QList<PluginGroup> getPluginList() const;

signals:
    void pluginDoubleClicked(QString plugin_name);

private slots:
    void refreshSearchWidget(QString str);

private:
    Ui::PluginLoader* ui;
    QList<PluginGroup> m_list;
    PluginTreeWidget* m_main_list;
    PluginFindWidget* m_search_list;
};
