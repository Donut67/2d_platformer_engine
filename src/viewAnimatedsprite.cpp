#include "../include/viewAnimatedsprite.h"
#include "myFunctions.h"

void ViewAnimatedSprite::show(){
    DrawRectangleRec(Rectangle{_pos.x + 2.0f, _pos.y + 1.0f, 446, 25.0f}, Color{80, 80, 80, 255});
    DrawTextEx(_font, "ANIMATED SPRITE", Vector2{_pos.x + 7.0f, _pos.y + 2.0f}, 24, 1, WHITE);

    DrawTextEx(_font, "FILE NAME:", Vector2{_pos.x + 7.0f, _pos.y + 25.0f}, 24, 1, WHITE);
    DrawTextEx(_font, _filename.c_str(), Vector2{_pos.x + 7.0f + 180.0f, _pos.y + 25.0f}, 24, 1, WHITE);

    DrawTextEx(_font, "TILE SIZE:", Vector2{_pos.x + 7.0f, _pos.y + 50.0f}, 24, 1, WHITE);
    DrawTextEx(_font, ftostr(_tileSize.x, 2).c_str(), Vector2{_pos.x + 7.0f + 180.0f, _pos.y + 50.0f}, 24, 1, WHITE);
    DrawTextEx(_font, ftostr(_tileSize.y, 2).c_str(), Vector2{_pos.x + 7.0f + 300.0f, _pos.y + 50.0f}, 24, 1, WHITE);

    DrawTextEx(_font, "SCALE:", Vector2{_pos.x + 7.0f, _pos.y + 75.0f}, 24, 1, WHITE);
    DrawTextEx(_font, ftostr(_scale, 2).c_str(), Vector2{_pos.x + 7.0f + 180.0f, _pos.y + 75.0f}, 24, 1, WHITE);

    DrawLineEx(Vector2{_pos.x, _pos.y + 100.0f}, Vector2{_pos.x + 450, _pos.y + 100.0f}, 2, BLACK);
}

void ViewAnimatedSprite::update(const GameObject& go, const Vector2 &pos){
    _filename = go[AnimatedSprite()]->getFileName();
    _tileSize = go[AnimatedSprite()]->getTileSize();
    _scale = go[AnimatedSprite()]->getScale();
    _pos = pos;
    show();
}