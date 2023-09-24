#pragma once

#include <dsplab/controller.h>

namespace DspLab {

class Summator;
class SummatorCtrl : public Controller
{
public:
    SummatorCtrl();
    virtual ~SummatorCtrl();
    QWidget* window() final;

private:
    Summator* m_sum;
};

}   // namespace DspLab
