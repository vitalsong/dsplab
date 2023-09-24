#include <dsplab/export.h>

#include <plugin-factory.h>

//-------------------------------------------------------------
DspLab::IPluginFactory* instance()
{
    static DspLab::PluginFactory factory;
    return &factory;
}

//-------------------------------------------------------------
const char* name()
{
    return "UDP Sender";
}

//-------------------------------------------------------------
uint64_t guid()
{
    return 409171044;
}

//-------------------------------------------------------------
const char* version()
{
    return "0.0.1";
}
