#include "../include/viewTransform.h"
#include "myFunctions.h"

void ViewTransform::show(){
    DrawRectangleRec(Rectangle{_pos.x + 2.0f, _pos.y + 1.0f, 446, 25.0f}, Color{80, 80, 80, 255});
    DrawTextEx(_font, "TRANSFORM", Vector2{_pos.x + 7.0f, _pos.y + 2.0f}, 24, 1, WHITE);

    DrawTextEx(_font, "POSITION:", Vector2{_pos.x + 7.0f, _pos.y + 2.0f + 25.0f}, 24, 1, WHITE);
    DrawTextEx(_font, ftostr(_position.x, 2).c_str(), Vector2{_pos.x + 7.0f + 160.0f, _pos.y + 2.0f + 25.0f}, 24, 1, WHITE);
    DrawTextEx(_font, ftostr(_position.y, 2).c_str(), Vector2{_pos.x + 7.0f + 280.0f, _pos.y + 2.0f + 25.0f}, 24, 1, WHITE);

    DrawTextEx(_font, "SCALE:   ", Vector2{_pos.x + 7.0f, _pos.y + 2.0f + 50.0f}, 24, 1, WHITE);
    DrawTextEx(_font, ftostr(_scale.x, 2).c_str(), Vector2{_pos.x + 7.0f + 160.0f, _pos.y + 2.0f + 50.0f}, 24, 1, WHITE);
    DrawTextEx(_font, ftostr(_scale.y, 2).c_str(), Vector2{_pos.x + 7.0f + 280.0f, _pos.y + 2.0f + 50.0f}, 24, 1, WHITE);

    DrawLineEx(Vector2{_pos.x, _pos.y + 75.0f}, Vector2{_pos.x + 450, _pos.y + 75.0f}, 2, BLACK);
}

void ViewTransform::update(const GameObject& go, const Vector2 &pos){
    _position = go[TransformComp()]->getGlobalPosition();
    _scale = go[TransformComp()]->getScale();
    _pos = pos;
    show();
}