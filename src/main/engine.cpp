#include "raylib.h"
#include "../include/engineScene.h"
#include "../include/tracker.h"
#include "../include/viewTransform.h"
#include "../include/viewGameobject.h"
#include "../include/viewAnimatedsprite.h"
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
    shared_ptr<Observer> v_go = make_shared<ViewGameobject>();
    shared_ptr<Observer> v_t  = make_shared<ViewTransform>();
    shared_ptr<Observer> v_as  = make_shared<ViewAnimatedSprite>();

    SetTargetFPS(144);

    manager.update();
    while(!WindowShouldClose()){
        // manager.update();
        // if(WindowShouldClose() || manager.WindowShouldClose()) exitWindow = true;
        BeginDrawing();
            ClearBackground(GRAY);
            manager.draw();
            if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                shared_ptr<GameObject> go = manager.findReversePre(GetMousePosition());
                t.clearObservers();
                if(go != nullptr) {
                    t.setObserver(v_go);
                    for(auto c : go->getComponents()){
                        if(dynamic_pointer_cast<TransformComp>(c) != nullptr) t.setObserver(v_t);
                        if(dynamic_pointer_cast<AnimatedSprite>(c) != nullptr) t.setObserver(v_as);
                    }
                    t.setGameObject(go);
                }
            }
            t.notify();
            DrawText((to_string(GetFPS()) + " FPS").c_str(), GetScreenWidth() - 100, 10, 36, GREEN);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
