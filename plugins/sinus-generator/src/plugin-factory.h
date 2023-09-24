#pragma once

#include <dsplab/i_plugin-factory.h>
#include "sinus-generator-ctrl.h"

namespace DspLab {

class PluginFactory : public IPluginFactory
{
public:
    PluginFactory();
    virtual ~PluginFactory();
    std::shared_ptr<IController> create();
};

PluginFactory::PluginFactory()
{}

PluginFactory::~PluginFactory()
{}

std::shared_ptr<IController> PluginFactory::create()
{
    return std::shared_ptr<IController>(new SinusGeneratorCtrl);
}

}   // namespace DspLab
