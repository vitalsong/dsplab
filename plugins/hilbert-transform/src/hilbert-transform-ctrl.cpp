#include "hilbert-transform-ctrl.h"
#include "hilbert-transform.h"

using namespace DspLab;

//------------------------------------------------------------------------------------------
HilbertTransformCtrl::HilbertTransformCtrl()
{
    m_hilbert = new HilbertTransform();
    this->setDeviceModel(m_hilbert);
}

//------------------------------------------------------------------------------------------
HilbertTransformCtrl::~HilbertTransformCtrl()
{
    delete m_hilbert;
}

//------------------------------------------------------------------------------------------
QWidget* HilbertTransformCtrl::window()
{
    return nullptr;
}
