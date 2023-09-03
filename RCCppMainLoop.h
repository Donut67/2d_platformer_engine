#pragma once

#include "RuntimeInclude.h"
RUNTIME_MODIFIABLE_INCLUDE; //recompile runtime files when this changes
#include "platformerEngineBasics.h"

// abstract interface to our RCCppMainLoop class, using I at end to denote Interface
struct RCCppMainLoopI
{
    virtual void MainLoop() = 0;
    virtual bool WindowShouldClose() = 0;
    virtual void edit_tilemap(shared_ptr<TileMap> size) = 0;
};