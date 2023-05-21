#include "raylib.h"
#include "../include/engineScene.h"
#include "../include/inspector.h"
#include "../include/hierarchy.h"
#include <cstdlib>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

using namespace std;

void reloadObservers(Tracker &t, shared_ptr<GameObject> go, vector<shared_ptr<Observer>> ol);

int main(){
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(0, 0, "2D Platformer Engine");
    SetExitKey(KEY_NULL);
    MaximizeWindow();
    
    EngineScene manager(vector<string>{"prova.meta"}, 0);
    Inspector inspector(Vector2{GetScreenWidth() - 450.0f, 0.0f});
    Hierarchy hierarchy(Vector2{0.0f, 0.0f}, manager);

    SetTargetFPS(60);

    manager.update();
    while(!WindowShouldClose()){
        // if(canUpdate) manager.update();
        // if(WindowShouldClose() || manager.WindowShouldClose()) exitWindow = true;
        inspector.update();
        hierarchy.update();

        // if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && !inspector.isClicked() && !inspector.isMoving() && !hierarchy.isClicked() && !hierarchy.isMoving()) {
        //     shared_ptr<GameObject> go = manager.findReversePre(GetMousePosition());
        //     inspector.setObservers(go);
        // } else 
        if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && hierarchy.objectSelected()) inspector.setObservers(hierarchy.getSelected());

        BeginDrawing();
            // Draw Scene
            ClearBackground(GRAY);
            manager.draw();
            inspector.draw();
            hierarchy.draw();

            DrawFPS(GetScreenWidth() - 100, 10);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}

void reloadObservers(Tracker &t, shared_ptr<GameObject> go, vector<shared_ptr<Observer>> ol){
    t.clearObservers();
    if(go != nullptr) {
        t.setObserver(ol[0]);
        for(auto c : go->getComponents()){
            if(dynamic_pointer_cast<TransformComp>(c) != nullptr) t.setObserver(ol[1]);
            if(dynamic_pointer_cast<AnimatedSprite>(c) != nullptr) t.setObserver(ol[2]);
            if(dynamic_pointer_cast<RigidBody>(c) != nullptr) t.setObserver(ol[3]);
            if(dynamic_pointer_cast<AABB>(c) != nullptr) t.setObserver(ol[4]);
        }
        t.setGameObject(go);
    }
}