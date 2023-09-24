#include "controller.h"

#include <QtCore>

using namespace DspLab;

//-------------------------------------------------------------------------------------------------
Controller::Controller()
  : m_device(nullptr)
{
    //nothing to do
}

//-------------------------------------------------------------------------------------------------
Controller::~Controller()
{
    //nothing to do
}

//-------------------------------------------------------------------------------------------------
IDevice* Controller::device()
{
    return m_device;
}

//-------------------------------------------------------------------------------------------------
QVariantMap Controller::paramList() const
{
    QVariantMap list;
    return list;
}

//-------------------------------------------------------------------------------------------------
void Controller::setParam(QVariantMap param)
{
    Q_UNUSED(param);
}

//-------------------------------------------------------------------------------------------------
int Controller::valuesCount() const
{
    return 0;
}

//-------------------------------------------------------------------------------------------------
std::vector<DisplayValue> Controller::valuesList() const
{
    std::vector<DisplayValue> empty;
    return empty;
}

//-------------------------------------------------------------------------------------------------
void Controller::setDeviceModel(IDevice* dev)
{
    m_device = dev;
}
