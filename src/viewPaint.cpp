#include "../include/viewPaint.h"
#include "myFunctions.h"

void ViewPaint::show(){
    DrawRectangleRec(Rectangle{_pos.x + 2.0f, _pos.y + 1.0f, 446, 25.0f}, Color{80, 80, 80, 255});
    DrawTextEx(_font, "Paint", Vector2{_pos.x + 7.0f, _pos.y + 2.0f}, 24, 1, WHITE);

    Color c = _paint->getColor();
    DrawTextEx(_font, "COLOR:", Vector2{_pos.x + 7.0f, _pos.y + 2.0f + 25.0f}, 24, 1, WHITE);
    DrawRectangleRec(Rectangle{_pos.x + 7.0f + 160.0f, _pos.y + 27.0f, 200.0f, 20.0f}, Color{c.r, c.g, c.b, 255});
    DrawRectangleRec(Rectangle{_pos.x + 7.0f + 160.0f, _pos.y + 46.0f, 200.0f, 3.0f}, RED);
    DrawRectangleRec(Rectangle{_pos.x + 7.0f + 160.0f, _pos.y + 46.0f, c.a / 255 * 200.0f, 3.0f}, WHITE);
    DrawRectangleLines(_pos.x + 7.0f + 160.0f , _pos.y + 27.0f, 200.0f, 23.0f, BLACK);

    DrawLineEx(Vector2{_pos.x, _pos.y + 52.0f}, Vector2{_pos.x + 450, _pos.y + 52.0f}, 2, BLACK);

    if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
        if(CheckCollisionPointRec(GetMousePosition(), Rectangle{_pos.x + 7.0f + 160.0f, _pos.y + 27.0f, 200.0f, 20.0f})) {
            using std::placeholders::_1;
            _cw = make_shared<ColorWheel>(Vector2{_pos.x + 7.0f + 160.0f , _pos.y + 52.0f}, c, bind( &Paint::setColor, _paint, _1 ));
        }
    }
}

void ViewPaint::update(shared_ptr<GameObject> go, const Vector2 &pos){
    _paint = go->getComponent<Paint>();
    _pos = pos;

    if(_cw != nullptr) {
        _cw->update();
        if(_cw->isClosed()) _cw = nullptr;
    }
    show();
    if(_cw != nullptr) _cw->draw();
}