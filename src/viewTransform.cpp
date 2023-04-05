#include "../include/viewTransform.h"
#include "myFunctions.h"

void ViewTransform::show(){
    DrawRectangle(10, _top, 450, 110, LIGHTGRAY);
    DrawTextEx(_font, "TRANSFORM", Vector2{20.0f, _top + 10.0f}, 30, 1, WHITE);
    DrawLine(10, _top + 40.0f, 460, _top + 40.0f, GRAY);

    DrawTextEx(_font, "POSITION:", Vector2{20.0f, _top + 40.0f}, 30, 1, WHITE);
    DrawTextEx(_font, ftostr(_pos.x, 2).c_str(), Vector2{180.0f, _top + 40.0f}, 30, 1, WHITE);
    DrawTextEx(_font, ftostr(_pos.y, 2).c_str(), Vector2{300.0f, _top + 40.0f}, 30, 1, WHITE);

    DrawTextEx(_font, "SCALE:   ", Vector2{20.0f, _top + 70.0f}, 30, 1, WHITE);
    DrawTextEx(_font, ftostr(_scale.x, 2).c_str(), Vector2{180.0f, _top + 70.0f}, 30, 1, WHITE);
    DrawTextEx(_font, ftostr(_scale.y, 2).c_str(), Vector2{300.0f, _top + 70.0f}, 30, 1, WHITE);
}

void ViewTransform::update(const GameObject& go, const float &height){
    _pos = go[TransformComp()]->getGlobalPosition();
    _scale = go[TransformComp()]->getScale();
    _top = height;
    show();
}