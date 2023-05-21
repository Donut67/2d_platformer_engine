#include "../include/viewSprite.h"
#include "myFunctions.h"

void ViewSprite::show(){
    DrawRectangleRec(Rectangle{_pos.x + 2.0f, _pos.y + 1.0f, 446, 25.0f}, Color{80, 80, 80, 255});
    DrawTextEx(_font, "Sprite", Vector2{_pos.x + 7.0f, _pos.y + 2.0f}, 24, 1, WHITE);

    DrawTextEx(_font, "IMAGE:", Vector2{_pos.x + 7.0f, _pos.y + 2.0f + 25.0f}, 24, 1, WHITE);

    DrawTextEx(_font, " FILENAME:", Vector2{_pos.x + 7.0f, _pos.y + 2.0f + 50.0f}, 24, 1, WHITE);
    DrawTextEx(_font, _filename != ""? _filename.c_str() : "Empty", Vector2{_pos.x + 7.0f + 160.0f, _pos.y + 2.0f + 50.0f}, 24, 1, WHITE);

    DrawTextEx(_font, " SOURCE:", Vector2{_pos.x + 7.0f, _pos.y + 2.0f + 75.0f}, 24, 1, WHITE);
    if(_texture != nullptr) DrawTextureEx(*_texture, _pos, 0.0f, _texture->height / 100, WHITE);
    else {
        DrawRectanglePro(Rectangle{_pos.x + 7.0f + 160.0f, _pos.y + 2.0f + 75.0f + 100 - 7.1f, 131.4, 10}, Vector2{0, 0}, -45, RED);
        DrawRectangleLines(_pos.x + 7.0f + 160.0f, _pos.y + 2.0f + 75.0f, 100, 100, WHITE);
    }

    DrawTextEx(_font, "SCALE:   ", Vector2{_pos.x + 7.0f, _pos.y + 2.0f + 175.0f}, 24, 1, WHITE);
    DrawTextEx(_font, ftostr(_scale, 2).c_str(), Vector2{_pos.x + 7.0f + 160.0f, _pos.y + 2.0f + 175.0f}, 24, 1, WHITE);

    DrawLineEx(Vector2{_pos.x, _pos.y + 202.0f}, Vector2{_pos.x + 450, _pos.y + 202.0f}, 2, BLACK);
}

void ViewSprite::update(shared_ptr<GameObject> go, const Vector2 &pos){
    _scale = go->getComponent<Sprite>()->scale();
    _filename = go->getComponent<Sprite>()->filename();
    _texture = go->getComponent<Sprite>()->texture();
    _pos = pos;
    show();
}