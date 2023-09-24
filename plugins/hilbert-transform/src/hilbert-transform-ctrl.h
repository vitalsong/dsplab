#pragma once

#include <dsplab/controller.h>

namespace DspLab {

class HilbertTransform;
class HilbertTransformCtrl : public Controller
{
public:
    HilbertTransformCtrl();
    virtual ~HilbertTransformCtrl();
    QWidget* window() final;

private:
    HilbertTransform* m_hilbert;
};

}   // namespace DspLab
