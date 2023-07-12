#pragma once

#include "ObjectInterfacePerModule.h"
#include "RuntimeInclude.h"
RUNTIME_MODIFIABLE_INCLUDE; //recompile runtime files when this changes

struct RCCppMainLoopI;
struct ImGuiContext;
class EngineBaseEditor;
struct IRuntimeObjectSystem;

static SystemTable*& g_pSys = PerModuleInterface::g_pSystemTable;

struct SystemTable
{
    RCCppMainLoopI*       pRCCppMainLoopI      = NULL;
    ImGuiContext*         pImContext           = NULL;
    EngineBaseEditor*     pEBEditor            = NULL;
    IRuntimeObjectSystem* pRuntimeObjectSystem = NULL;
};