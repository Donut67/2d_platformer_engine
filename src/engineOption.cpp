#include "raylib.h"
#include "engineOption.h"
#include <cstdlib>
#include <iostream>

using namespace std;

void EngineOption::update() {
    _hovered = CheckCollisionPointRec(GetMousePosition(), Rectangle{_position.x, _position.y, _size.x, _size.y});

    if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && _hovered) _on_click();
}
void EngineOption::move(const Vector2 &movement) {
    _position = Vector2{_position.x + movement.x, _position.y + movement.y};
}

void EngineOption::draw() {
    DrawRectangle(_position.x, _position.y, _size.x, _size.y, _hovered? BLUE : Color{0, 0, 0, 0});

    DrawTextEx(_font, _name.c_str(), Vector2{_position.x + 5.0f, _position.y + 1.0f}, _size.y - 2.0f, 0, WHITE);
}