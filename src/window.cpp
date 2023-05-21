#include "raylib.h"
#include "window.h"
#include <cstdlib>
#include <iostream>

using namespace std;

void Window::update(){
    _isClicked = false;
    if(!_moving) _prev_position = _position;
    if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
        if(CheckCollisionPointRec(GetMousePosition(), Rectangle{_position.x, _position.y, _size.x, 30})) {
            _dragging = true;
            _prevMousePosition = GetMousePosition();
        }else if(CheckCollisionPointRec(GetMousePosition(), Rectangle{_position.x, _position.y + 30, _size.x, _size.y - 30})) _isClicked = true;
    }else if(IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
        _dragging = false;
        
        if(_sticky) {
            if(_position.x < 50.0f) _position.x = 0;
            if(_position.y < 50.0f) _position.y = 0;
            if(_position.x + _size.x > GetScreenWidth() - 50.0f) _position.x = GetScreenWidth() - _size.x;
            if(_position.y + _size.y > GetScreenHeight() - 50.0f) _position.y = GetScreenHeight() - _size.y;
        }
    }

    if(_dragging) {
        Vector2 move{_prevMousePosition.x - GetMousePosition().x, _prevMousePosition.y - GetMousePosition().y};
        Vector2 p = Vector2{_position.x - move.x, _position.y - move.y};
        if(p.x < 0) p.x = 0;
        if(p.y < 0) p.y = 0;
        if(p.x + _size.x > GetScreenWidth()) p.x = GetScreenWidth() - _size.x;
        if(p.y + _size.y > GetScreenHeight()) p.y = GetScreenHeight() - _size.y;

        _position = p;

        _prevMousePosition = GetMousePosition();
    }
}

void Window::move(const Vector2 &movement) {
    _moving = movement.x != 0 || movement.y != 0;
    if(_moving) {
        _prev_position = _position;
        _position = Vector2{_position.x + movement.x, _position.y + movement.y};
    }
}

void Window::draw(){
    DrawRectangleRec(Rectangle{_position.x, _position.y, _size.x, _size.y}, Color{100, 100, 100, 255});
    DrawRectangleLinesEx(Rectangle{_position.x, _position.y, _size.x, _size.y}, 2, BLACK);

    if(_title != "") {
        DrawRectangleV(_position, Vector2{_size.x, _font_size + 2.0f}, _dragging? Color{50, 50, 50, 255} : BLACK);
        DrawTextEx(_font, _title.c_str(), Vector2{_position.x + 5, _position.y + 1}, _font_size, 0, WHITE);
    }
}

Vector2 Window::movement() const{
    return Vector2{_position.x - _prev_position.x, _position.y - _prev_position.y};
}

bool Window::isDragging() const {
    return _dragging;
}

bool Window::isClicked() const {
    return _isClicked;
}

string Window::getTitle() const {
    return _title;
}

Vector2 Window::getPosition() const {
    return _position;
}