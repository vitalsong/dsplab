#include <dsplab/export.h>

#include "plugin-factory.h"

//-------------------------------------------------------------------------------------------------
DspLab::IPluginFactory* instance()
{
    static DspLab::PluginFactory factory;
    return &factory;
}

//-------------------------------------------------------------------------------------------------
const char* name()
{
    return "Imag";
}

//-------------------------------------------------------------------------------------------------
uint64_t guid()
{
    return 3008170955;
}

//-------------------------------------------------------------------------------------------------
const char* version()
{
    return "0.0.1";
}
