#pragma once

#include <dsplab/controller.h>

namespace DspLab {

class SinadDev;
class SinadCtrl : public Controller
{
public:
    SinadCtrl();
    virtual ~SinadCtrl();
    QWidget* window() final;
    int valuesCount() const final;
    std::vector<DisplayValue> valuesList() const final;

private:
    SinadDev* m_unit;
};

} /// DspLab
