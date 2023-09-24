#pragma once

#include <dsplab/i_plugin-factory.h>

#include <spectrum-analyzer-ctrl.h>

namespace DspLab {

/*!
 * \brief Реализация фабрики плагинов
 */
class PluginFactory : public IPluginFactory
{
public:
    PluginFactory();
    virtual ~PluginFactory();
    std::shared_ptr <IController> create() final;
};

PluginFactory::PluginFactory()
{
    //nothing to do
}

PluginFactory::~PluginFactory()
{
    //nothing to do
}

std::shared_ptr <IController>
PluginFactory::create()
{
    return std::shared_ptr <IController> (new SpectrumAnalyzerCtrl);
}

} /// DspLab


