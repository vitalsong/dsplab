#pragma once

#include <QMenuBar>

class QMenu;
class QAction;

/*!
 * \brief Панель меню приложения
 */
class MenuBar : public QMenuBar
{
    Q_OBJECT

public:
    MenuBar(QWidget* parent = nullptr);
    virtual ~MenuBar();

signals:
    void openChecked();
    void saveAsChecked();
    void saveChecked();
    void exitChecked();
    void startAllChecked();
    void stopAllChecked();
    void deleteAllChecked();
    void refreshPluginsChecked();
};
