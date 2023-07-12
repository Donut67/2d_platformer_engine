#include "RCCppMainLoop.h"

#include "raylib.h"
#include "platformerEngineUI.h"

#include "RuntimeLinkLibrary.h"
RUNTIME_COMPILER_LINKLIBRARY("C:\\Users\\Usuario\\Documents\\Games\\vcpkg\\packages\\raylib_x64-windows\\lib\\raylib.lib");
// RUNTIME_COMPILER_LINKLIBRARY("raylib.lib");

#include "IObject.h"
#include "SystemTable.h"
#include "ISimpleSerializer.h"
#include "IRuntimeObjectSystem.h"
#include "ObjectInterfacePerModule.h"

// /*
#include "RuntimeSourceDependency.h"
RUNTIME_COMPILER_SOURCEDEPENDENCY_FILE("platformerEngineUI", ".cpp");
RUNTIME_COMPILER_SOURCEDEPENDENCY_FILE("platformerEngineBasics", ".cpp");
// */

// RCC++ uses interface Id's to distinguish between different classes
// here we have only one, so we don't need a header for this enum and put it in the same
// source code file as the rest of the code
enum InterfaceIDEnumRCCppEngine
{
    IID_IRCCPP_MAIN_LOOP = IID_ENDInterfaceID, // IID_ENDInterfaceID from IObject.h InterfaceIDEnum

    IID_ENDInterfaceIDEnumRCCppEngine
};

struct RCCppMainLoop : RCCppMainLoopI, TInterface<IID_IRCCPP_MAIN_LOOP, IObject>
{
    // values
    bool  show_menu_bar = true;
    Color bg_color      = GRAY;

    // data for compiling window
    static constexpr double SHOW_AFTER_COMPILE_TIME = 3.0f;
    double compileStartTime = -SHOW_AFTER_COMPILE_TIME;
    double compileEndTime = -SHOW_AFTER_COMPILE_TIME;
    unsigned int compiledModules = 0;

    RCCppMainLoop() {
        g_pSys->pRCCppMainLoopI = this;
    }

    void Serialize(ISimpleSerializer* pSerializer) override {
        SERIALIZE( show_menu_bar );
        SERIALIZE( bg_color );
    }

    void MainLoop() override {
        shared_ptr<EngineSprite> new_item = nullptr;

        // /*
        // Setting compiling info
        double time = GetTime();
        bool bCompiling = g_pSys->pRuntimeObjectSystem->GetIsCompiling();
        double timeSinceLastCompile = time - compileEndTime;
        if (bCompiling || timeSinceLastCompile < SHOW_AFTER_COMPILE_TIME) {
            if (bCompiling) {
                if (timeSinceLastCompile > SHOW_AFTER_COMPILE_TIME) compileStartTime = time;
                compileEndTime = time; // ensure always updated
            }
            bool bCompileOk = g_pSys->pRuntimeObjectSystem->GetLastLoadModuleSuccess();
            
            Color windowBgCol = GREEN;
            if (!bCompiling) {
                if (bCompileOk) windowBgCol = GREEN;
                else windowBgCol = RED;
            }
            new_item = make_shared<EngineSprite>(Vector2{ GetScreenWidth() - 300.0f, GetScreenHeight() - 20.0f }, Vector2{ 300.0f, 20.0f }, 1.0f, windowBgCol);
            
            char text[80];

            if (bCompiling) sprintf(text, "Compiling... time %.2fs", (float)(time - compileStartTime));
            else if (bCompileOk) sprintf(text, "Compiling... time %.2fs", (float)(time - compileStartTime));
            else sprintf(text, "Compiling... time %.2fs", (float)(time - compileStartTime));

            new_item->addObject(make_shared<EngineLabel>(Vector2{ 0.0f, 0.0f }, Vector2{ 300.0f, 20.0f }, text, 18, Padding(10, 0)));
        }
        // */

        // Updating the base editor
        g_pSys->pEBEditor->update();
        if (new_item != nullptr) new_item->update();

        BeginDrawing();
            // Clearing and drawing the base editor
            
            ClearBackground(GRAY);
            g_pSys->pEBEditor->draw();
            if (new_item != nullptr) new_item->draw();
            // menu_bar->draw();
        EndDrawing();
    }
};

REGISTERSINGLETON(RCCppMainLoop, true); 