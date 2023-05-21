#include "../include/viewAnimatedsprite.h"
#include "myFunctions.h"

void ViewAnimatedSprite::show(){
    DrawRectangleRec(Rectangle{_pos.x + 2.0f, _pos.y + 1.0f, 446, 25.0f}, Color{80, 80, 80, 255});
    DrawTextEx(_font, "ANIMATED SPRITE", Vector2{_pos.x + 7.0f, _pos.y + 2.0f}, 24, 1, WHITE);

    DrawTextEx(_font, "IMAGE:", Vector2{_pos.x + 7.0f, _pos.y + 2.0f + 25.0f}, 24, 1, WHITE);
    DrawTextEx(_font, " FILENAME:", Vector2{_pos.x + 7.0f, _pos.y + 2.0f + 50.0f}, 24, 1, WHITE);
    DrawTextEx(_font, _filename != ""? _filename.c_str() : "Empty", Vector2{_pos.x + 7.0f + 160.0f, _pos.y + 2.0f + 50.0f}, 24, 1, WHITE);

    DrawTextEx(_font, " SOURCE:", Vector2{_pos.x + 7.0f, _pos.y + 2.0f + 75.0f}, 24, 1, WHITE);
    if(_texture != nullptr) {
        DrawTextureEx(*_texture, Vector2{_pos.x + 7.0f + 160.0f, _pos.y + 2.0f + 75.0f}, 0.0f, (100.0f / _texture->height), WHITE);
        // cout << 100 / _texture->height << "\n";
    } else {
        DrawRectanglePro(Rectangle{_pos.x + 7.0f + 160.0f, _pos.y + 2.0f + 75.0f + 100 - 7.1f, 131.4, 10}, Vector2{0, 0}, -45, RED);
        DrawRectangleLines(_pos.x + 7.0f + 160.0f, _pos.y + 2.0f + 75.0f, 100, 100, WHITE);
    }

    DrawTextEx(_font, "SCALE:   ", Vector2{_pos.x + 7.0f, _pos.y + 2.0f + 175.0f}, 24, 1, WHITE);
    DrawTextEx(_font, ftostr(_scale, 2).c_str(), Vector2{_pos.x + 7.0f + 160.0f, _pos.y + 2.0f + 175.0f}, 24, 1, WHITE);

    DrawTextEx(_font, "TILE SIZE:", Vector2{_pos.x + 7.0f, _pos.y + 202.0f}, 24, 1, WHITE);
    DrawTextEx(_font, ftostr(_tileSize.x, 2).c_str(), Vector2{_pos.x + 7.0f + 180.0f, _pos.y + 202.0f}, 24, 1, WHITE);
    DrawTextEx(_font, ftostr(_tileSize.y, 2).c_str(), Vector2{_pos.x + 7.0f + 300.0f, _pos.y + 202.0f}, 24, 1, WHITE);


    DrawLineEx(Vector2{_pos.x, _pos.y + 227.0f}, Vector2{_pos.x + 450, _pos.y + 227.0f}, 2, BLACK);
}

void ViewAnimatedSprite::update(shared_ptr<GameObject> go, const Vector2 &pos){
    _filename = go->getComponent<AnimatedSprite>()->getFileName();
    _texture = go->getComponent<AnimatedSprite>()->getTexture();
    _tileSize = go->getComponent<AnimatedSprite>()->getTileSize();
    _scale = go->getComponent<AnimatedSprite>()->getScale();
    _go = go;
    _pos = pos;
    show();
}