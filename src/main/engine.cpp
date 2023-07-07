#define CR_HOST // required in the host only and before including cr.h
#include "cr.h"

#include "raylib.h"
#include "platformerEngineUI.h"
#include <cstdlib>
#include <iostream>
#include <functional>

using namespace std;

int main(){
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(0, 0, "2D Platformer Engine");
    SetExitKey(KEY_NULL);
    MaximizeWindow();
    
    Scene scene("resources/prova.meta");
    EngineBaseEditor base_editor(scene);

    SetTargetFPS(60);

    while(!WindowShouldClose()){
        base_editor.update();
        BeginDrawing();
            ClearBackground(GRAY);
            base_editor.draw();
        EndDrawing();
    }

    CloseWindow();
}