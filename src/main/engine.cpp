#include "raylib.h"
#include "platformerEngineUI.h"
#include <cstdlib>
#include <iostream>

using namespace std;

int main() {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(0, 0, "2D Platformer Engine");
    SetExitKey(KEY_NULL);
    MaximizeWindow();

    // Initialize RCC++
    // RCCppInit();

    // Initializing the base editor
    //EngineBaseEditor* base_editor = new EngineBaseEditor();
    shared_ptr<EngineObject> base = make_shared<EngineObject>(Vector2{0.0f, 0.0f}, Vector2{0.0f, 0.0f});
    base->addObject(make_shared<EngineAux>("okay", Vector2{0.0f, 0.0f}, Vector2{55.0f, 20.0f}));

    // Setting the base editor in the system table
    // g_SystemTable.pEBEditor = base_editor;

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        // RCCppUpdate();

        // g_SystemTable.pRCCppMainLoopI->MainLoop();

        base->update();
        BeginDrawing();
            ClearBackground(GRAY);
            base->draw();
        EndDrawing();
    }

    // Cleaning up the closing
    // RCCppCleanup();
    CloseWindow();
}