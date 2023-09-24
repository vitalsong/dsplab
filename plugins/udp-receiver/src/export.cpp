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
    return "UDP Receiver";
}

//-------------------------------------------------------------
uint64_t guid()
{
    return 109171104;
}

//-------------------------------------------------------------
const char* version()
{
    return "0.0.1";
}
