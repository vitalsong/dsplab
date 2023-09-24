#include "real-transform-ctrl.h"
#include "real-transform.h"

using namespace DspLab;

//-------------------------------------------------------------------------------------------------
RealTransformCtrl::RealTransformCtrl()
{
    m_real = new RealTransform();
    this->setDeviceModel(m_real);
}

//-------------------------------------------------------------------------------------------------
RealTransformCtrl::~RealTransformCtrl()
{
    delete m_real;
}

//-------------------------------------------------------------------------------------------------
QWidget* RealTransformCtrl::window()
{
    return nullptr;
}
