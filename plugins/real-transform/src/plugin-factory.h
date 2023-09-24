#pragma once

#include <dsplab/i_plugin-factory.h>
#include "real-transform-ctrl.h"

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
    return std::shared_ptr<IController>(new RealTransformCtrl());
}

}   // namespace DspLab
