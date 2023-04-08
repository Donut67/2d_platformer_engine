#include "raylib.h"
#include "../include/engineScene.h"
#include "../include/tracker.h"
#include "../include/viewTransform.h"
#include "../include/viewGameobject.h"
#include "../include/viewAnimatedsprite.h"
#include "../include/viewRigidbody.h"
#include "../include/viewAABB.h"
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
    
    EngineScene manager(vector<string>{"creator.meta"}, 0);
    // bool exitWindow = false;

    bool canUpdate, isPendingAprove, aprovingGO;
    canUpdate = isPendingAprove = aprovingGO = false;
    bool isSelecting = true;
    Font font = LoadFont("resources/monogram.ttf");
    shared_ptr<GameObject> go = nullptr;

    Tracker t;
    vector<shared_ptr<Observer>> observerList{
        make_shared<ViewGameobject>(),
        make_shared<ViewTransform>(),
        make_shared<ViewAnimatedSprite>(),
        make_shared<ViewRigidBody>(),
        make_shared<ViewAABB>()
    };

    SetTargetFPS(144);

    manager.update();
    while(!WindowShouldClose()){
        if(canUpdate) manager.update();
        // if(WindowShouldClose() || manager.WindowShouldClose()) exitWindow = true;
        BeginDrawing();
            // Draw Scene
            ClearBackground(GRAY);
            manager.draw();

            if(isPendingAprove) {
                // 
                Vector2 pos{GetScreenWidth() / 2.0f - GetScreenWidth() / 4.0f, 300};
                Vector2 size{GetScreenWidth() / 2.0f, 100};

                DrawRectangle(pos.x, pos.y, size.x, size.y, LIGHTGRAY);
                string text;
                if(aprovingGO) text = "Do you want to add a game object to '" + (go != nullptr? go->name() : "root") + "'? ";
                else text = "Do you want to add a new component to '" + go->name() + "'? ";
                float length = text.size() * 36 / 2;
                Vector2 this_pos = Vector2{pos.x + (size.x / 2) - (length / 2), pos.y};
                DrawTextEx(font, text.c_str(), Vector2{this_pos.x, this_pos.y}, 36, 0, GRAY);

                if(IsKeyPressed(KEY_ENTER)) {
                    isPendingAprove = aprovingGO = false;
                    if(go != nullptr) go = manager.setNewChildTo(go);
                    reloadObservers(t, go, observerList);
                }else if(IsKeyPressed(KEY_ESCAPE)) isPendingAprove = false;
            } else if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) { // Check Mouse click
                if(GetMousePosition().y > GetScreenHeight() - 30) {
                    if(GetMousePosition().x < 60) canUpdate = true;
                    else if(GetMousePosition().x < 120) canUpdate = false;
                    else if(GetMousePosition().x < 310) isPendingAprove = aprovingGO = true;
                    else if(GetMousePosition().x < 485 && go != nullptr) isPendingAprove = true;
                } else if(GetMousePosition().y < GetScreenHeight() - 30 && isSelecting) {
                    go = manager.findReversePre(GetMousePosition());
                    reloadObservers(t, go, observerList);
                }
            }
            // t.notify();
            manager.drawTreePostorder(Vector2{10, 10});

            DrawRectangle(0, GetScreenHeight() - 30, GetScreenWidth(), 30, LIGHTGRAY);

            // Play button
            DrawPoly(Vector2{30, GetScreenHeight() - 15.0f}, 3, 10, -90, GRAY);
            DrawLine(60, GetScreenHeight() - 30, 60, GetScreenHeight(), GRAY);

            // Pause button
            DrawRectangle(83, GetScreenHeight() - 25.0f, 6, 20, GRAY);
            DrawRectangle(92, GetScreenHeight() - 25.0f, 6, 20, GRAY);
            DrawLine(120, GetScreenHeight() - 30, 120, GetScreenHeight(), GRAY);

            // Add Gameobject button
            DrawTextEx(font, "New GameObject", Vector2{130, GetScreenHeight() - 27.0f}, 24, 1, GRAY);
            DrawLine(310, GetScreenHeight() - 30, 310, GetScreenHeight(), GRAY);

            if(go != nullptr) {
                // Add Component button
                DrawTextEx(font, "Add Component", Vector2{320, GetScreenHeight() - 27.0f}, 24, 1, GRAY);
                DrawLine(485, GetScreenHeight() - 30, 485, GetScreenHeight(), GRAY);
            }

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