#include <dsplab/export.h>
#include "plugin-factory.h"

//-------------------------------------------------------------
DspLab::IPluginFactory* instance()
{
    static DspLab::PluginFactory factory;
    return &factory;
}

//-------------------------------------------------------------
const char* name()
{
    return "Hex Viewer";
}

//-------------------------------------------------------------
uint64_t guid()
{
    return 1909170921;
}

//-------------------------------------------------------------
const char* version()
{
    return "0.0.1";
}
