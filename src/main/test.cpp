#include "raylib.h"
#include "../include/engineLabeledGraph.h"
#include <cstdlib>
#include <iostream>
#include <functional>

using namespace std;

int main(){
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(0, 0, "2D Platformer Engine");
    SetExitKey(KEY_NULL);
    MaximizeWindow();

    WindowLabeledGraph lg(Vector2{0.0, 0.0});
    
    SetTargetFPS(144);
    while(!WindowShouldClose()){
        lg.update();
        BeginDrawing();
            ClearBackground(GRAY);
            lg.draw();
        EndDrawing();
    }

    CloseWindow();
}