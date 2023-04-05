#include "../include/viewGameobject.h"

void ViewGameobject::show() {
    DrawRectangle(10, _top, 450, 50, LIGHTGRAY);
    DrawTextEx(_font, _name.c_str(), Vector2{20.0f, _top + 10.0f}, 30, 1, WHITE);
}

void ViewGameobject::update(const GameObject& go, const float &height) {
    _name = go.name();
    _top = height;
    show();
}