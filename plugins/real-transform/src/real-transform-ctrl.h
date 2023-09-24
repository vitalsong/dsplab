#pragma once

#include <dsplab/controller.h>

namespace DspLab {

class RealTransform;
class RealTransformCtrl : public Controller
{
public:
    RealTransformCtrl();
    virtual ~RealTransformCtrl();
    QWidget* window() final;

private:
    RealTransform* m_real;
};

}   // namespace DspLab
