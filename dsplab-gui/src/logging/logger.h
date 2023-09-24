#pragma once

#include <QListWidget>

namespace Logger {

/*!
 * \brief Размещение объекта логера и привязка виджета отображения
 * \param w Виджет для отображения
 */
void instance(QListWidget* w);

/*!
 * \brief Тип события логера
 */
enum EventType
{
    Info,      ///< информационное сообщение
    Warning,   ///< предупреждение
    Error,     ///< ошибка
};

/*!
 * \brief Печать сообщения
 * \param type Тип события
 * \param msg Сообщение
 */
void print(EventType type, QString msg);

}   // namespace Logger
