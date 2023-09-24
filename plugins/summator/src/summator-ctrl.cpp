#include <summator-ctrl.h>
#include <summator.h>

using namespace DspLab;

//-------------------------------------------------------------------------------------------------
SummatorCtrl::SummatorCtrl()
{
    m_sum = new Summator();
    this->setDeviceModel(m_sum);
}

//-------------------------------------------------------------------------------------------------
SummatorCtrl::~SummatorCtrl()
{
    delete m_sum;
}

//-------------------------------------------------------------------------------------------------
QWidget* SummatorCtrl::window()
{
    return nullptr;
}
