#include "../include/viewGameobject.h"

void ViewGameobject::show() {
    DrawTextEx(_font, _name.c_str(), Vector2{_pos.x + 7.0f, _pos.y}, 24, 1, WHITE);
    DrawLineEx(Vector2{_pos.x, _pos.y + 25.0f}, Vector2{_pos.x + 450, _pos.y + 25.0f}, 2, BLACK);
}

void ViewGameobject::update(const GameObject& go, const Vector2 &pos) {
    _name = go.name();
    _pos = pos;
    show();
}