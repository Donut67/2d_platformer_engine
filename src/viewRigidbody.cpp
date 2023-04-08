#include "../include/viewRigidbody.h"
#include "myFunctions.h"

void ViewRigidBody::show(){
    DrawRectangle(10, _top, 450, 80, LIGHTGRAY);
    DrawTextEx(_font, "RIGID BODY", Vector2{20.0f, _top + 10.0f}, 30, 1, WHITE);
    DrawLine(10, _top + 40.0f, 460, _top + 40.0f, GRAY);

    DrawTextEx(_font, "VELOCITY:", Vector2{20.0f, _top + 40.0f}, 30, 1, WHITE);
    DrawTextEx(_font, ftostr(_velocity.x, 2).c_str(), Vector2{180.0f, _top + 40.0f}, 30, 1, WHITE);
    DrawTextEx(_font, ftostr(_velocity.y, 2).c_str(), Vector2{300.0f, _top + 40.0f}, 30, 1, WHITE);
}

void ViewRigidBody::update(const GameObject& go, const float &height){
    _velocity = go[RigidBody()]->getVelocity();
    _top = height;
    show();
}