#include "raylib.h"
#include "../include/engineScene.h"
#include "../include/tracker.h"
#include "../include/viewTransform.h"
#include <cstdlib>
#include <iostream>
#include <memory>
#include <string>

using namespace std;

int main(){
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(0, 0, "2D Platformer Engine");
    SetExitKey(KEY_NULL);
    MaximizeWindow();
    
    EngineScene manager(vector<string>{"creator.meta"}, 0);
    // bool exitWindow = false;

    Tracker t;
    shared_ptr<Observer> view = make_shared<ViewTransform>("View Transorm");
    t.setObserver(view);
    int actual = 0;

    SetTargetFPS(144);

    manager.update();
    while(!WindowShouldClose()){
        // manager.update();
        // if(WindowShouldClose() || manager.WindowShouldClose()) exitWindow = true;
        BeginDrawing();
            ClearBackground(GRAY);
            manager.draw();
            if(GetKeyPressed() == KEY_TAB) {
                t.setGameObject(manager.getNthGameObject(actual));
                actual ++;
            }
            t.notify();
            DrawText((to_string(GetFPS()) + " FPS").c_str(), GetScreenWidth() - 100, 10, 36, GREEN);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
