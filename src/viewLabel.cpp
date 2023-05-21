#include "../include/viewLabel.h"
#include "myFunctions.h"

void ViewLabel::show(){
    DrawRectangleRec(Rectangle{_pos.x + 2.0f, _pos.y + 1.0f, 446, 25.0f}, Color{80, 80, 80, 255});
    DrawTextEx(_font, "Label", Vector2{_pos.x + 7.0f, _pos.y + 2.0f}, 24, 1, WHITE);
    DrawRectangle(_pos.x + 400.0f, _pos.y + 2.0f, 21, 21, RED);
    DrawPoly(Vector2{_pos.x + 410.0f, _pos.y + 12.0f}, 3, 8, 180, WHITE);
    DrawRectangle(_pos.x + 425.0f, _pos.y + 2.0f, 21, 21, RED);
    DrawPoly(Vector2{_pos.x + 435.0f, _pos.y + 12.0f}, 3, 8, 0, WHITE);

    DrawTextEx(_font, "TEXT:", Vector2{_pos.x + 7.0f, _pos.y + 2.0f + 25.0f}, 24, 1, WHITE);
    _text.setPosition(Vector2{_pos.x + 7.0f + 160.0f, _pos.y + 2.0f + 25.0f});
    _text.draw();

    DrawTextEx(_font, "FONT SIZE:", Vector2{_pos.x + 7.0f, _pos.y + 2.0f + 50.0f}, 24, 1, WHITE);
    _fontSize.setPosition(Vector2{_pos.x + 7.0f + 160.0f, _pos.y + 2.0f + 50.0f});
    _fontSize.draw();

    char align = _label->getAllign();
    DrawTextEx(_font, "TEXT ALIGN:", Vector2{_pos.x + 7.0f, _pos.y + 2.0f + 75.0f}, 24, 1, WHITE);
    DrawRectangleRec(Rectangle{_pos.x + 7.0f + 160.0f, _pos.y + 2.0f + 75.0f, 23, 23}, align == 'l'? BLUE : GRAY);
    DrawRectangleRec(Rectangle{_pos.x + 7.0f + 163.0f, _pos.y + 2.0f + 78.0f, 17, 4}, LIGHTGRAY);
    DrawRectangleRec(Rectangle{_pos.x + 7.0f + 163.0f, _pos.y + 2.0f + 85.0f, 13, 4}, LIGHTGRAY);
    DrawRectangleRec(Rectangle{_pos.x + 7.0f + 163.0f, _pos.y + 2.0f + 92.0f,  7, 4}, LIGHTGRAY);

    DrawRectangleRec(Rectangle{_pos.x + 7.0f + 190.0f, _pos.y + 2.0f + 75.0f, 23, 23}, align == 'c'? BLUE : GRAY);
    DrawRectangleRec(Rectangle{_pos.x + 7.0f + 193.0f, _pos.y + 2.0f + 78.0f, 17, 4}, LIGHTGRAY);
    DrawRectangleRec(Rectangle{_pos.x + 7.0f + 195.0f, _pos.y + 2.0f + 85.0f, 13, 4}, LIGHTGRAY);
    DrawRectangleRec(Rectangle{_pos.x + 7.0f + 198.0f, _pos.y + 2.0f + 92.0f,  6, 4}, LIGHTGRAY);

    DrawRectangleRec(Rectangle{_pos.x + 7.0f + 220.0f, _pos.y + 2.0f + 75.0f, 23, 23}, align == 'r'? BLUE : GRAY);
    DrawRectangleRec(Rectangle{_pos.x + 7.0f + 223.0f, _pos.y + 2.0f + 78.0f, 17, 4}, LIGHTGRAY);
    DrawRectangleRec(Rectangle{_pos.x + 7.0f + 227.0f, _pos.y + 2.0f + 85.0f, 13, 4}, LIGHTGRAY);
    DrawRectangleRec(Rectangle{_pos.x + 7.0f + 233.0f, _pos.y + 2.0f + 92.0f,  7, 4}, LIGHTGRAY);

    Color c = _label->getColor();
    DrawTextEx(_font, "COLOR:", Vector2{_pos.x + 7.0f, _pos.y + 2.0f + 100.0f}, 24, 1, WHITE);
    DrawRectangleRec(Rectangle{_pos.x + 7.0f + 160.0f , _pos.y + 102.0f, 200.0f, 20.0f}, Color{c.r, c.g, c.b, 255});
    DrawRectangleRec(Rectangle{_pos.x + 7.0f + 160.0f , _pos.y + 121.0f, 200.0f, 3.0f}, RED);
    DrawRectangleRec(Rectangle{_pos.x + 7.0f + 160.0f , _pos.y + 121.0f, c.a / 255 * 200.0f, 3.0f}, WHITE);
    DrawRectangleLines(_pos.x + 7.0f + 160.0f , _pos.y + 102.0f, 200.0f, 23.0f, BLACK);

    DrawLineEx(Vector2{_pos.x, _pos.y + 127.0f}, Vector2{_pos.x + 450, _pos.y + 127.0f}, 2, BLACK);

}

void ViewLabel::update(shared_ptr<GameObject> go, const Vector2 &pos){
    _label = go->getComponent<Label>();
    _pos = pos;

    _fontSize.setValue(_label->getfontSize());
    _fontSize.update();
    _label->setFontSize(_fontSize.getValue());

    _text.setValue(_label->getText());
    _text.update();
    _label->setText(_text.getValue());

    if(_cw != nullptr) {
        _cw->update();
        if(_cw->isClosed()) _cw = nullptr;
    }
    show();
    if(_cw != nullptr) _cw->draw();
    
    if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
        if(CheckCollisionPointRec(GetMousePosition(), Rectangle{_pos.x + 400.0f, _pos.y + 2.0f, 21, 21})) {
            go->moveComponentUp<Label>();
            _on_click();
        }else if(CheckCollisionPointRec(GetMousePosition(), Rectangle{_pos.x + 425.0f, _pos.y + 2.0f, 21, 21})) {
            go->moveComponentDown<Label>();
            _on_click();
        }else if(CheckCollisionPointRec(GetMousePosition(), Rectangle{_pos.x + 7.0f + 160.0f, _pos.y + 2.0f + 75.0f, 23, 23})) _label->setAlign('l');
        else if(CheckCollisionPointRec(GetMousePosition(), Rectangle{_pos.x + 7.0f + 190.0f, _pos.y + 2.0f + 75.0f, 23, 23})) _label->setAlign('c');
        else if(CheckCollisionPointRec(GetMousePosition(), Rectangle{_pos.x + 7.0f + 220.0f, _pos.y + 2.0f + 75.0f, 23, 23})) _label->setAlign('r');
        else if(CheckCollisionPointRec(GetMousePosition(), Rectangle{_pos.x + 7.0f + 160.0f , _pos.y + 102.0f, 200.0f, 20.0f})) {
            using std::placeholders::_1;
            _cw = make_shared<ColorWheel>(Vector2{_pos.x + 7.0f + 160.0f , _pos.y + 121.0f}, _label->getColor(), bind( &Label::setColor, _label, _1 ));
        }
    }
}