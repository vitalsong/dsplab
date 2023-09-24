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
    return "Gain";
}

//-------------------------------------------------------------------------------------------------
uint64_t guid()
{
    return 2301181658;
}

//-------------------------------------------------------------------------------------------------
const char* version()
{
    return "0.0.1";
}
