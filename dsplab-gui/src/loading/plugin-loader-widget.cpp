#include <loading/plugin-loader-widget.h>
#include <loading/plugin-tree-widget.h>
#include <loading/plugin-find-widget.h>

#include "ui_plugin-loader.h"

//-------------------------------------------------------------------------------------------------
PluginLoaderWidget::PluginLoaderWidget(QWidget* parent)
  : QWidget(parent)
  , ui(new Ui::PluginLoader)
  , m_main_list(new PluginTreeWidget(this))
  , m_search_list(new PluginFindWidget(this))
{
    ui->setupUi(this);

    //добавляем дерево плагинов
    ui->treeLayout->addWidget(m_main_list);

    //добавляем лист результата поиска
    ui->findLayout->addWidget(m_search_list);

    //режим по-умолчанию (список групп)
    ui->modeStackedWidget->setCurrentIndex(1);

    //изменение в строке поиска
    connect(ui->findPluginEdit, &QLineEdit::textChanged, this, &PluginLoaderWidget::refreshSearchWidget);

    //двойное нажатие по элементу
    auto func = [&](const QModelIndex& index) {
        emit pluginDoubleClicked(index.data().toString());
    };

    connect(m_main_list, &PluginTreeWidget::doubleClicked, func);
    connect(m_search_list, &PluginFindWidget::doubleClicked, func);
}

//-------------------------------------------------------------------------------------------------
PluginLoaderWidget::~PluginLoaderWidget()
{
    delete ui;
}

//-------------------------------------------------------------------------------------------------
void PluginLoaderWidget::setPluginList(QList<PluginLoaderWidget::PluginGroup> list)
{
    //сохраняем список
    m_list = list;

    //очистка дерева элементов
    m_main_list->clear();

    //проход по группам
    for (const PluginLoaderWidget::PluginGroup& group : m_list) {
        //создаем вкладку группы
        QTreeWidgetItem* groupTree = new QTreeWidgetItem(m_main_list);
        groupTree->setText(0, group.first);
        groupTree->setBackgroundColor(0, QColor(230, 230, 230));
        m_main_list->addTopLevelItem(groupTree);

        //проход по элементам группы
        for (const QString& name : group.second) {
            //создаем элемент устройства
            QTreeWidgetItem* devTree = new QTreeWidgetItem(groupTree);

            //имя элемента
            devTree->setText(0, name);

            //добавляем элемент в группу
            groupTree->addChild(devTree);
        }
    }

    //сортировка
    m_main_list->sortByColumn(0, Qt::AscendingOrder);
}

//-------------------------------------------------------------------------------------------------
QList<PluginLoaderWidget::PluginGroup> PluginLoaderWidget::getPluginList() const
{
    return m_list;
}

//-------------------------------------------------------------------------------------------------
void PluginLoaderWidget::refreshSearchWidget(QString str)
{
    if (str.isEmpty()) {
        ui->modeStackedWidget->setCurrentIndex(1);
    } else {
        //очистка прошлого списка
        m_search_list->clear();

        QRegExp regex(str);
        for (const PluginLoaderWidget::PluginGroup& group : m_list) {
            for (const QString& name : group.second) {
                //если подстрока не была найдена
                if (regex.indexIn(name) == -1) {
                    continue;
                }

                //добавляем элемент в список
                QListWidgetItem* item = new QListWidgetItem();
                item->setText(name);
                m_search_list->addItem(item);
            }
        }

        //выставляем виджет с результатами поиска
        ui->modeStackedWidget->setCurrentIndex(0);
    }
}
