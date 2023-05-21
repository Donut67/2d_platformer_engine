#include "../include/viewButton.h"
#include "myFunctions.h"

void ViewButton::show(){
    DrawRectangleRec(Rectangle{_pos.x + 2.0f, _pos.y + 1.0f, 446, 25.0f}, Color{80, 80, 80, 255});
    DrawTextEx(_font, "BUTTON", Vector2{_pos.x + 7.0f , _pos.y + 2.0f}, 24, 1, WHITE);

    DrawTextEx(_font, "COLOR:", Vector2{_pos.x + 7.0f , _pos.y + 25.0f}, 24, 1, WHITE);

    DrawTextEx(_font, "  BASE:", Vector2{_pos.x + 7.0f , _pos.y + 50.0f}, 24, 1, WHITE);
    DrawRectangleRec(Rectangle{_pos.x + 7.0f + 160.0f , _pos.y + 52.0f, 200.0f, 20.0f}, Color{_color.r, _color.g, _color.b, 255});
    DrawRectangleRec(Rectangle{_pos.x + 7.0f + 160.0f , _pos.y + 71.0f, 200.0f, 3.0f}, RED);
    DrawRectangleRec(Rectangle{_pos.x + 7.0f + 160.0f , _pos.y + 71.0f, _color.a / 255 * 200.0f, 3.0f}, WHITE);
    DrawRectangleLines(_pos.x + 7.0f + 160.0f , _pos.y + 52.0f, 200.0f, 23.0f, BLACK);

    DrawTextEx(_font, "  HOEVER:", Vector2{_pos.x + 7.0f , _pos.y + 75.0f}, 24, 1, WHITE);
    DrawRectangleRec(Rectangle{_pos.x + 7.0f + 160.0f , _pos.y + 77.0f, 200.0f, 20.0f}, Color{_hoverColor.r, _hoverColor.g, _hoverColor.b, 255});
    DrawRectangleRec(Rectangle{_pos.x + 7.0f + 160.0f , _pos.y + 96.0f, 200.0f, 3.0f}, RED);
    DrawRectangleRec(Rectangle{_pos.x + 7.0f + 160.0f , _pos.y + 96.0f, _hoverColor.a / 255 * 200.0f, 3.0f}, WHITE);
    DrawRectangleLines(_pos.x + 7.0f + 160.0f , _pos.y + 77.0f, 200.0f, 23.0f, BLACK);
    
    DrawLineEx(Vector2{_pos.x, _pos.y + 102.0f}, Vector2{_pos.x + 450, _pos.y + 102.0f}, 2, BLACK);
}

void ViewButton::update(shared_ptr<GameObject> go, const Vector2 &pos){
    _color = go->getComponent<Button>()->getColor();
    _hoverColor = go->getComponent<Button>()->getHoverColor();
    _pos = pos;
    show();
}