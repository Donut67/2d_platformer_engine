#include "../include/viewAnimatedsprite.h"
#include "myFunctions.h"

void ViewAnimatedSprite::show(){
    DrawRectangle(10, _top, 450, 140, LIGHTGRAY);
    DrawTextEx(_font, "ANIMATED SPRITE", Vector2{20.0f, _top + 10.0f}, 30, 1, WHITE);
    DrawLine(10, _top + 40.0f, 460, _top + 40.0f, GRAY);

    DrawTextEx(_font, "FILE NAME:", Vector2{20.0f, _top + 40.0f}, 30, 1, WHITE);
    DrawTextEx(_font, _filename.c_str(), Vector2{180.0f, _top + 40.0f}, 30, 1, WHITE);

    DrawTextEx(_font, "TILE SIZE:", Vector2{20.0f, _top + 70.0f}, 30, 1, WHITE);
    DrawTextEx(_font, ftostr(_tileSize.x, 2).c_str(), Vector2{180.0f, _top + 70.0f}, 30, 1, WHITE);
    DrawTextEx(_font, ftostr(_tileSize.y, 2).c_str(), Vector2{300.0f, _top + 70.0f}, 30, 1, WHITE);

    DrawTextEx(_font, "SCALE:   ", Vector2{20.0f, _top + 100.0f}, 30, 1, WHITE);
    DrawTextEx(_font, ftostr(_scale, 2).c_str(), Vector2{180.0f, _top + 100.0f}, 30, 1, WHITE);
}

void ViewAnimatedSprite::update(const GameObject& go, const float &height){
    _filename = go[AnimatedSprite()]->getFileName();
    _tileSize = go[AnimatedSprite()]->getTileSize();
    _scale = go[AnimatedSprite()]->getScale();
    _top = height;
    show();
}