#include <menu/menu-bar.h>

#include <QMenu>
#include <QDebug>
#include <QAction>
#include <QMessageBox>

//-------------------------------------------------------------------------------------------------
MenuBar::MenuBar(QWidget* parent)
  : QMenuBar(parent)
{
    //вкладка "файл"
    auto menu_file = new QMenu("Файл", this);
    menu_file->addAction("Открыть...", this, &MenuBar::openChecked);
    menu_file->addAction("Сохранить как...", this, &MenuBar::saveAsChecked);
    menu_file->addAction("Сохранить", this, &MenuBar::saveChecked), QKeySequence(tr("Ctrl+s"));
    menu_file->addAction("Выход", this, &MenuBar::exitChecked);

    //вкладка "запуск"
    auto menu_start = new QMenu("Запуск", this);
    menu_start->addAction("Запустить все", this, &MenuBar::startAllChecked);
    menu_start->addAction("Остановить все", this, &MenuBar::stopAllChecked);
    menu_start->addAction("Удалить всё", this, &MenuBar::deleteAllChecked);

    //вкладка "вид"
    auto menu_view = new QMenu("Вид", this);

    //вкладка "настройки"
    auto menu_setting = new QMenu("Настройки", this);
    menu_setting->addAction("Обновить плагины", this, &MenuBar::refreshPluginsChecked);

    //вкладка "справка"
    auto menu_info = new QMenu("Справка", this);
    menu_info->addAction("О программе");
    menu_info->addAction("Помощь");

    //добавляем элементы меню
    this->addMenu(menu_file);
    this->addMenu(menu_start);
    this->addMenu(menu_view);
    this->addMenu(menu_setting);
    this->addMenu(menu_info);

    //отображаем
    this->show();
}

//-------------------------------------------------------------------------------------------------
MenuBar::~MenuBar()
{
    //nothing to do
}
