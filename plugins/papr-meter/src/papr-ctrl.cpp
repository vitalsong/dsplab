#include "papr-ctrl.h"
#include "papr-dev.h"

using namespace DspLab;

//-------------------------------------------------------------------------------------------------
PaprCtrl::PaprCtrl()
{
    m_unit = new PaprDev();
    this->setDeviceModel(m_unit);
}

//-------------------------------------------------------------------------------------------------
PaprCtrl::~PaprCtrl()
{
    delete m_unit;
}

//-------------------------------------------------------------------------------------------------
QWidget* PaprCtrl::window()
{
    return nullptr;
}

//-------------------------------------------------------------------------------------------------
int PaprCtrl::valuesCount() const
{
    return 1;
}

//-------------------------------------------------------------------------------------------------
std::vector<DisplayValue> PaprCtrl::valuesList() const
{
    return {{"PAPR", m_unit->value(), 2, DisplayValue::VALUE_UNIT, "dB"}};
}
