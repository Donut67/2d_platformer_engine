#include "raylib.h"
#include "scene.h"
#include <cstdlib>
#include <iostream>
#include <memory>
#include <string>

using namespace std;

int main(){
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(0, 0, "Prova");
    SetExitKey(KEY_NULL);
    MaximizeWindow();
    
    Scene manager(vector<string>{"creator.meta"}, 0);
    bool exitWindow = false;

    SetTargetFPS(144);

    while(!exitWindow){
        manager.update();
        if(WindowShouldClose() || manager.WindowShouldClose()) exitWindow = true;
        BeginDrawing();
            ClearBackground(GRAY);
            manager.draw();
            DrawText((to_string(GetFPS()) + " FPS").c_str(), 10, 10, 36, GREEN);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
