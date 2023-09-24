#pragma once

#include <dsplab/i_controller.h>

namespace DspLab {

/*!
 * \brief Абстрактный класс контроллера устройств
 */
class Controller : public IController
{
public:
    Controller();
    virtual ~Controller() override;
    IDevice* device() final;

    //заглушки для вирт.функций
    QVariantMap paramList() const override;
    void setParam(QVariantMap param) override;
    int valuesCount() const override;
    std::vector<DisplayValue> valuesList() const override;

protected:
    /*!
     * \brief Назначение модели устройства
     * Каждый объект контроллера должен агрегировать в себе базовую модель,
     * и назначать её через эту функцию (либо явно)
     * \param dev Указатель на интерфейс модели
     */
    void setDeviceModel(IDevice* dev);

private:
    IDevice* m_device;
};

}   // namespace DspLab
