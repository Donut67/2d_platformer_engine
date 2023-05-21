#include "raylib.h"
#include "colorWheel.h"
#include <cstdlib>
#include <iostream>
#include <string>

void ColorWheel::update() {
    Window::update();
    _cancel_hover = CheckCollisionPointRec(GetMousePosition(), Rectangle{_position.x + 160, _position.y + 150, 80, 20});
    _confirm_hover = CheckCollisionPointRec(GetMousePosition(), Rectangle{_position.x + 255, _position.y + 150, 80, 20});

    if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
        if(_confirm_hover) {
            _on_confirm(_current);
            _close = true;
        }
    }
    if(IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
        if(CheckCollisionPointRec(GetMousePosition(), Rectangle{_position.x + 20, _position.y + 35, _size.x - 65.0f, 20})) {
            _current.r = (GetMousePosition().x - (_position.x + 20)) * 255.0f / (_size.x - 65.0f - 2);
        }else if(CheckCollisionPointRec(GetMousePosition(), Rectangle{_position.x + 20, _position.y + 57, _size.x - 65.0f, 20})) {
            _current.g = (GetMousePosition().x - (_position.x + 20)) * 255.0f / (_size.x - 65.0f - 2);
        }else if(CheckCollisionPointRec(GetMousePosition(), Rectangle{_position.x + 20, _position.y + 79, _size.x - 65.0f, 20})) {
            _current.b = (GetMousePosition().x - (_position.x + 20)) * 255.0f / (_size.x - 65.0f - 2);
        }else if(CheckCollisionPointRec(GetMousePosition(), Rectangle{_position.x + 20, _position.y + 101, _size.x - 65.0f, 20})){
            _current.a = (GetMousePosition().x - (_position.x + 20)) * 255.0f / (_size.x - 65.0f - 2);
        }
    }
}

void ColorWheel::draw() {
    Window::draw();

    float block_size = _size.x - 65.0f;

    DrawTextEx(_font, "R", Vector2{_position.x + 5, _position.y + 35}, 18, 0, BLACK);
    DrawRectangleLinesEx(Rectangle{_position.x + 20, _position.y + 35, block_size, 20}, 2, BLACK);
    DrawRectangle(_position.x + 20 + _current.r / 255.0f * (block_size - 2), _position.y + 35, 3, 20, RED);
    DrawTextEx(_font, to_string(_current.r).c_str(), Vector2{_position.x + 20 + block_size + 10, _position.y + 35}, 18, 0, BLACK);


    DrawTextEx(_font, "G", Vector2{_position.x + 5, _position.y + 57}, 18, 0, BLACK);
    DrawRectangleLinesEx(Rectangle{_position.x + 20, _position.y + 57, block_size, 20}, 2, BLACK);
    DrawRectangle(_position.x + 20 + _current.g / 255.0f * (block_size - 2), _position.y + 57, 3, 20, RED);
    DrawTextEx(_font, to_string(_current.g).c_str(), Vector2{_position.x + 20 + block_size + 10, _position.y + 57}, 18, 0, BLACK);


    DrawTextEx(_font, "B", Vector2{_position.x + 5, _position.y + 79}, 18, 0, BLACK);
    DrawRectangleLinesEx(Rectangle{_position.x + 20, _position.y + 79, block_size, 20}, 2, BLACK);
    DrawRectangle(_position.x + 20 + _current.b / 255.0f * (block_size - 2), _position.y + 79, 3, 20, RED);
    DrawTextEx(_font, to_string(_current.b).c_str(), Vector2{_position.x + 20 + block_size + 10, _position.y + 79}, 18, 0, BLACK);


    DrawTextEx(_font, "A", Vector2{_position.x + 5, _position.y + 101}, 18, 0, BLACK);
    DrawRectangleLinesEx(Rectangle{_position.x + 20, _position.y + 101, block_size, 20}, 2, BLACK);
    DrawRectangle(_position.x + 20 + _current.a / 255.0f * (block_size - 2), _position.y + 101, 3, 20, RED);
    DrawTextEx(_font, to_string(_current.a).c_str(), Vector2{_position.x + 20 + block_size + 10, _position.y + 101}, 18, 0, BLACK);

    DrawRectangleRec(Rectangle{_position.x + 20, _position.y + 123, block_size, 20}, _current);
    DrawRectangleLinesEx(Rectangle{_position.x + 20, _position.y + 123, block_size, 20}, 2, BLACK);

    DrawRectangleRec(Rectangle{_position.x + 160, _position.y + 150, 80, 20}, _cancel_hover? BLUE : GRAY);
    DrawTextEx(_font, "CANCEL", Vector2{_position.x + 175, _position.y + 150}, 18, 0, BLACK);
    DrawRectangleRec(Rectangle{_position.x + 255, _position.y + 150, 80, 20}, _confirm_hover? BLUE : GRAY);
    DrawTextEx(_font, "CONFIRM", Vector2{_position.x + 265, _position.y + 150}, 18, 0, BLACK);
}