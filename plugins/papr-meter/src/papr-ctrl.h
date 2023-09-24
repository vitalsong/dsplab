#pragma once

#include <dsplab/controller.h>

namespace DspLab {

class PaprDev;
class PaprCtrl : public Controller
{
public:
    PaprCtrl();
    virtual ~PaprCtrl();
    QWidget* window() final;
    int valuesCount() const final;
    std::vector<DisplayValue> valuesList() const final;

private:
    PaprDev* m_unit;
};

}   // namespace DspLab
