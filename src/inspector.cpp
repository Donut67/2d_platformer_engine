#include "raylib.h"
#include "inspector.h"
#include <cstdlib>
#include <iostream>

using namespace std;

void Inspector::update() {
    Window::update();

    bool hoverButton = CheckCollisionPointRec(GetMousePosition(), Rectangle{_buttonPosition.x, _buttonPosition.y, _buttonsize.x, _buttonsize.y});

    if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && hoverButton){
        _options = make_shared<EngineOptionList>(Vector2{_buttonPosition.x, _buttonPosition.y + 30.0f}, _buttonsize.x, 18, 
            vector<pair<string, function<void()>>>{
                {"Label", bind(&Inspector::addLabel, this)},
                {"Button", bind(&Inspector::addButton, this)},
                {"Sprite", bind(&Inspector::addSprite, this)},
                {"Animated Sprite", bind(&Inspector::addAnimatedSprite, this)}
            }
            // vector<string>{"sprite", "animatedsprite", "button", "label", "paint", "new behaviour"}
        );
    }
    

    if(_options != nullptr) {
        _options->update();

        // string name = _options->getSelected();
        // if(name != "") {
        //     if(name == "tilemap"){
        //         _go->addComponent(make_shared<TileMap>(TileMap(TileSet(nullptr, ""), Vector2{100, 100}, 1, "")));
        //     }else if(name == "paint"){
        //         _go->addComponent(make_shared<Paint>(Paint(WHITE)));
        //     }else if(name == "animatedsprite"){
        //         _go->addComponent(make_shared<AnimatedSprite>(AnimatedSprite("", nullptr, 1, "", 1)));
        //     }else if(name == "aabb"){
        //         _go->addComponent(make_shared<AABB>(AABB(false, false)));
        //     }else if(name == "button"){
        //         _go->addComponent(make_shared<Button>(Button(WHITE, BLUE)));
        //     }else if(name == "label"){
        //         _go->addComponent(make_shared<Label>(Label("New Label", make_shared<Font>(_font), 'l', 18, WHITE)));
        //     }else if(name == "camera"){
        //         _go->addComponent(make_shared<TrackCamera>(TrackCamera()));
        //     }else if(name == "mosaic"){
        //         _go->addComponent(make_shared<Mosaic>(Mosaic(nullptr, 1)));
        //     }else if(name == "sprite"){
        //         _go->addComponent(make_shared<Sprite>(Sprite("", nullptr, 1)));
        //     }else if(name == "animation"){
        //         _go->addComponent(make_shared<Animation>(Animation("")));
        //     }else if(name == "rigidbody"){
        //         _go->addComponent(make_shared<RigidBody>(RigidBody()));
        //     }

            // setObservers(_go);
            // _options = nullptr;
        // }
        if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && !hoverButton) _options = nullptr;
    }
}

void Inspector::setObservers(shared_ptr<GameObject> go) {
    _tracker.clearObservers();
    if(go != nullptr) {
        _tracker.setObserver(_observerList[0]);
        for(auto c : go->getComponents()){
            if(dynamic_pointer_cast<TransformComp>(c) != nullptr) _tracker.setObserver(_observerList[1]);
            else if(dynamic_pointer_cast<AnimatedSprite>(c) != nullptr) _tracker.setObserver(_observerList[2]);
            else if(dynamic_pointer_cast<Sprite>(c) != nullptr) _tracker.setObserver(_observerList[3]);
            else if(dynamic_pointer_cast<RigidBody>(c) != nullptr) _tracker.setObserver(_observerList[4]);
            else if(dynamic_pointer_cast<AABB>(c) != nullptr) _tracker.setObserver(_observerList[5]);
            else if(dynamic_pointer_cast<Button>(c) != nullptr) _tracker.setObserver(_observerList[6]);
            else if(dynamic_pointer_cast<Label>(c) != nullptr) _tracker.setObserver(_observerList[7]);
            else if(dynamic_pointer_cast<Paint>(c) != nullptr) _tracker.setObserver(_observerList[8]);
        }
        _tracker.setGameObject(go);
        _go = go;
    }
}

void Inspector::reload() {
    setObservers(_go);
}

void Inspector::draw() {
    Window::draw();

    Vector2 pos = Vector2{_position.x, _position.y + 30.0f};

    _tracker.notify(pos);

    _buttonPosition = Vector2{pos.x + 2.0f + 50.0f, pos.y + 10};
    DrawRectangle(_buttonPosition.x, _buttonPosition.y, _buttonsize.x, _buttonsize.y, BLUE);

    Vector2 position = Vector2{pos.x + 60.0f, pos.y + 10.0f};
    string text = "Add Component";
    float length = text.size() * (28 / 9 * 5);
    Vector2 this_pos = Vector2{position.x + (_buttonsize.x / 2) - (length / 2), position.y};
    DrawTextEx(_font, text.c_str(), this_pos, 28, 0, WHITE);

    if(_options != nullptr) _options->draw();
}

void Inspector::addLabel(){
    _go->addComponent(make_shared<Label>(Label("New Label", make_shared<Font>(_font), 'l', 18, WHITE)));
    setObservers(_go);
}

void Inspector::addButton(){
    _go->addComponent(make_shared<Button>(Button(WHITE, BLUE)));
    setObservers(_go);
}

void Inspector::addSprite() {
    _go->addComponent(make_shared<Sprite>(Sprite("", nullptr, 1)));
    setObservers(_go);

}
void Inspector::addAnimatedSprite(){
    _go->addComponent(make_shared<AnimatedSprite>(AnimatedSprite("", nullptr, 1, "", 1)));
    setObservers(_go);
}