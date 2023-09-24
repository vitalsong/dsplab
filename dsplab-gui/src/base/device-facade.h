#pragma once

#include <QObject>

#include <memory>
#include <vector>

#include <qglobal.h>

class IGraphicsDevice;
class PortAdapter;
class QTimer;

namespace DspLab {
class IController;
}

/*!
 * \brief Класс "фасад" для реализации взаимодействия между устройством и его отображением на сцене
 */
class DeviceFacade : public QObject
{
    Q_OBJECT

public:
    DeviceFacade(std::shared_ptr<DspLab::IController> controller, std::shared_ptr<IGraphicsDevice> sceneView,
                 QString name);

    virtual ~DeviceFacade();

    IGraphicsDevice* sceneView() const;
    DspLab::IController* controller() const;

    QString name() const;

    /*!
     * \brief Идентификатор объекта
     * \return Идентификатор объекта
     */
    quint32 id() const;

    /*!
     * \brief Получить кол-во входных портов
     * \return Кол-во портов
     */
    int nin() const;

    /*!
     * \brief Получить кол-во выходных портов
     * \return Кол-во портов
     */
    int nout() const;

    /*!
     * \brief Получить входной порт устройства
     * \param num Номер входного порта
     * \return Указатель на класс-адаптер
     */
    PortAdapter* input(int num) const;

    /*!
     * \brief output Получить выходной порт устройства
     * \param num Номер выходного порта
     * \return Указатель на класс-адаптер
     */
    PortAdapter* output(int num) const;

    /*!
     * \brief Запуск устройства
     * \return Успех операции
     */
    void start();

    /*!
     * \brief Остановка устройства
     */
    void stop();

    /*!
     * \brief Отобразить графическую форму
     */
    void show();

    /*!
     * \brief Скрыть графическую форму
     */
    void hide();

    //конфигурирование
    QVariantMap paramList() const;
    void setParam(QVariantMap param);

private:
    void configurateIO();
    void configurateView();
    void updateStatus();

private slots:
    void updateState();

private:
    quint32 m_id;
    std::shared_ptr<DspLab::IController> m_dev;
    std::shared_ptr<IGraphicsDevice> m_view;
    QString m_name;
    QTimer* m_timer;

    typedef std::shared_ptr<PortAdapter> PortAdapterPtr;
    std::vector<PortAdapterPtr> m_in;
    std::vector<PortAdapterPtr> m_out;
};
