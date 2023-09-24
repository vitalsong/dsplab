#pragma once

#include <dsplab/controller.h>

namespace DspLab {

class ImagTransform;
class ImagTransformCtrl : public Controller
{
public:
    ImagTransformCtrl();
    virtual ~ImagTransformCtrl();
    QWidget* window() final;

private:
    ImagTransform* m_imag;
};

}   // namespace DspLab
