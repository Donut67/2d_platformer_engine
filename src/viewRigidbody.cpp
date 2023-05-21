#include "../include/viewRigidbody.h"
#include "myFunctions.h"

void ViewRigidBody::show(){
    DrawRectangleRec(Rectangle{_pos.x + 2.0f, _pos.y + 1.0f, 446, 25.0f}, Color{80, 80, 80, 255});
    DrawTextEx(_font, "RIGID BODY", Vector2{_pos.x + 7.0f , _pos.y + 2.0f}, 24, 1, WHITE);

    DrawTextEx(_font, "VELOCITY:", Vector2{_pos.x + 7.0f , _pos.y + 25.0f}, 24, 1, WHITE);
    DrawTextEx(_font, ftostr(_velocity.x, 2).c_str(), Vector2{_pos.x + 7.0f + 180.0f, _pos.y + 25.0f}, 24, 1, WHITE);
    DrawTextEx(_font, ftostr(_velocity.y, 2).c_str(), Vector2{_pos.x + 7.0f + 300.0f, _pos.y + 25.0f}, 24, 1, WHITE);
    
    DrawLineEx(Vector2{_pos.x, _pos.y + 52.0f}, Vector2{_pos.x + 450, _pos.y + 52.0f}, 2, BLACK);
}

void ViewRigidBody::update(shared_ptr<GameObject> go, const Vector2 &pos){
    _velocity = go->getComponent<RigidBody>()->getVelocity();
    _pos = pos;
    show();
}