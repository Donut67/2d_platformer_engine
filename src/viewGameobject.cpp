#include "../include/viewGameobject.h"

void ViewGameobject::show() {
    DrawRectangle(_pos.x + 5, _pos.y + 3, 20, 20, _enable? BLUE : GRAY);
    _name.setPosition(Vector2{_pos.x + 25.0f + 7.0f, _pos.y + 2.0f});
    _name.draw();

    DrawLineEx(Vector2{_pos.x, _pos.y + 27.0f}, Vector2{_pos.x + 450, _pos.y + 27.0f}, 2, BLACK);

    if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
        if(CheckCollisionPointRec(GetMousePosition(), Rectangle{_pos.x + 5, _pos.y + 3, 20, 20})) _enable = !_enable;
    }
}

void ViewGameobject::update(shared_ptr<GameObject> go, const Vector2 &pos) {
    _enable = go->isEnable();
    _pos = pos;

    _name.setValue(go->name());
    _name.update();
    go->setName(_name.getValue());

    show();

    go->setEnable(_enable);
}