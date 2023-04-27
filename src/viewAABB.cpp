#include "../include/viewAABB.h"
#include "myFunctions.h"

void ViewAABB::show(){
    DrawRectangleRec(Rectangle{_pos.x + 2.0f, _pos.y + 1.0f, 446, 25.0f}, Color{80, 80, 80, 255});
    DrawTextEx(_font, "AABB COLLISION", Vector2{_pos.x + 7.0f, _pos.y + 2.0f}, 24, 1, WHITE);

    DrawTextEx(_font, "COLLIDING:", Vector2{_pos.x + 7.0f, _pos.y + 25.0f}, 24, 1, WHITE);
    DrawTextEx(_font, _colliding? "true" : "false", Vector2{_pos.x + 7.0f + 180.0f, _pos.y + 25.0f}, 24, 1, WHITE);

    DrawTextEx(_font, "STATIC:", Vector2{_pos.x + 7.0f, _pos.y + 50.0f}, 24, 1, WHITE);
    DrawTextEx(_font, _static? "true" : "false", Vector2{_pos.x + 7.0f + 180.0f, _pos.y + 50.0f}, 24, 1, WHITE);

    DrawTextEx(_font, "TRANSPARENT:", Vector2{_pos.x + 7.0f, _pos.y + 75.0f}, 24, 1, WHITE);
    DrawTextEx(_font, _transparent? "true" : "false", Vector2{_pos.x + 7.0f + 180.0f, _pos.y + 75.0f}, 24, 1, WHITE);

    DrawLineEx(Vector2{_pos.x, _pos.y + 100.0f}, Vector2{_pos.x + 450, _pos.y + 100.0f}, 2, BLACK);
}

void ViewAABB::update(const GameObject& go, const Vector2 &pos){
    _colliding = go[AABB()]->isColliding();
    _static = go[AABB()]->getStatic();
    _transparent = go[AABB()]->getTransparent();
    _pos = pos;
    show();
}