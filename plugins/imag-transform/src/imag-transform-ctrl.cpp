#include "imag-transform-ctrl.h"
#include "imag-transform.h"

using namespace DspLab;

//-------------------------------------------------------------------------------------------------
ImagTransformCtrl::ImagTransformCtrl()
{
    m_imag = new ImagTransform();
    this->setDeviceModel(m_imag);
}

//-------------------------------------------------------------------------------------------------
ImagTransformCtrl::~ImagTransformCtrl()
{
    delete m_imag;
}

//-------------------------------------------------------------------------------------------------
QWidget* ImagTransformCtrl::window()
{
    return nullptr;
}
