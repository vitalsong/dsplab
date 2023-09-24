#include <sinad-ctrl.h>
#include <sinad-dev.h>

using namespace DspLab;

//-------------------------------------------------------------------------------------------------
SinadCtrl::SinadCtrl()
{
    m_unit = new SinadDev();
    this->setDeviceModel(m_unit);
}

//-------------------------------------------------------------------------------------------------
SinadCtrl::~SinadCtrl()
{
    delete m_unit;
}

//-------------------------------------------------------------------------------------------------
QWidget* SinadCtrl::window()
{
    return nullptr;
}

//-------------------------------------------------------------------------------------------------
int SinadCtrl::valuesCount() const
{
    return 1;
}

//-------------------------------------------------------------------------------------------------
std::vector<DisplayValue> SinadCtrl::valuesList() const
{
    return {{"sinad", m_unit->value(), 2, DisplayValue::VALUE_UNIT, "dB"}};
}
