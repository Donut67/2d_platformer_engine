#include "raylib.h"
#include "../include/platformerEngineUI.h"
#include <cstdlib>
#include <iostream>
#include <functional>

using namespace std;

int main(){
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(0, 0, "2D Platformer Engine");
    SetExitKey(KEY_NULL);
    MaximizeWindow();

    // shared_ptr<string> text = make_shared<string>("This is an input text");
    // shared_ptr<EngineObject> screen_object = make_shared<EngineInput<string>>(Vector2{0.0f, 0.0f}, Vector2{300.0f, 20.0f}, text);
    
    Scene scene("resources/prova.meta");
    EngineBaseEditor base_editor(scene);

    // Nary<pair<bool, shared_ptr<GameObject>>> hierarchyTree = convertToNewTree(*scene.getRoot());
    // vector<pair<float, pair<string, function<void()>>>> action_list = NaryToActionList(hierarchyTree);
    // shared_ptr<EngineObject> screen_object = make_shared<EngineFitter>(Vector2{0.0f, 0.0f}, Vector2{(float)GetScreenWidth(), (float)GetScreenHeight()}, Padding{0, 0, 0, 0}, 0, false);
    // {
    //     shared_ptr<EngineObject> new_object = make_shared<EngineFitter>(Vector2{0.0f, 0.0f}, Vector2{300.0f, 0.0f}, Padding{1, -1, -1, 1}, -1);
    //     screen_object->addObject(new_object);
    //     new_object->addObject(EngineHierarchy::getInstance());
    //     EngineHierarchy::getInstance()->setRoot(scene.getRoot());
    // }
    // {
    //     shared_ptr<EngineObject> new_object = make_shared<EngineFitter>(Vector2{0.0f, 0.0f}, Vector2{300.0f, 0.0f}, Padding{1, 1, -1, -1}, -1);
    //     screen_object->addObject(new_object);
    //     new_object->addObject(EngineInspector::getInstance());
    // }

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