#include <dsplab/export.h>
#include "plugin-factory.h"

//-------------------------------------------------------------
static DspLab::PluginFactory* g_factory = nullptr;

//-------------------------------------------------------------
DspLab::IPluginFactory* instance()
{
    static DspLab::PluginFactory factory;
    g_factory = &factory;
    return g_factory;
}

//-------------------------------------------------------------
const char* name()
{
    return "Hilbert Transform";
}

//-------------------------------------------------------------
uint64_t guid()
{
    return 2908171600;
}

//-------------------------------------------------------------
const char* version()
{
    return "0.0.1";
}
