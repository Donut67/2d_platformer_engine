#pragma once

#include "RuntimeInclude.h"
RUNTIME_MODIFIABLE_INCLUDE; //recompile runtime files when this changes

#include "ObjectInterfacePerModule.h"

struct RCCppMainLoopI;
class EngineBaseEditor;
struct IRuntimeObjectSystem;
struct ICompilerLogger;

static SystemTable*& g_pSys = PerModuleInterface::g_pSystemTable;

struct SystemTable
{
    RCCppMainLoopI*       pRCCppMainLoopI      = NULL;
    EngineBaseEditor*     pEBEditor            = NULL;
    IRuntimeObjectSystem* pRuntimeObjectSystem = NULL;
    ICompilerLogger*      pLogger              = NULL;
};