#include "../include/viewAABB.h"
#include "myFunctions.h"

void ViewAABB::show(){
    DrawRectangle(10, _top, 450, 140, LIGHTGRAY);
    DrawTextEx(_font, "AABB COLLISION", Vector2{20.0f, _top + 10.0f}, 30, 1, WHITE);
    DrawLine(10, _top + 40.0f, 460, _top + 40.0f, GRAY);

    DrawTextEx(_font, "COLLIDING:", Vector2{20.0f, _top + 40.0f}, 30, 1, WHITE);
    DrawTextEx(_font, _colliding? "true" : "false", Vector2{180.0f, _top + 40.0f}, 30, 1, WHITE);

    DrawTextEx(_font, "STATIC:", Vector2{20.0f, _top + 70.0f}, 30, 1, WHITE);
    DrawTextEx(_font, _static? "true" : "false", Vector2{180.0f, _top + 70.0f}, 30, 1, WHITE);

    DrawTextEx(_font, "TRANSPAR.:", Vector2{20.0f, _top + 100.0f}, 30, 1, WHITE);
    DrawTextEx(_font, _transparent? "true" : "false", Vector2{180.0f, _top + 100.0f}, 30, 1, WHITE);
}

void ViewAABB::update(const GameObject& go, const float &height){
    _colliding = go[AABB()]->isColliding();
    _static = go[AABB()]->getStatic();
    _transparent = go[AABB()]->getTransparent();
    _top = height;
    show();
}