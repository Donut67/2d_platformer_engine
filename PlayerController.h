#ifndef __PLAYERCONTROLLER_H__
#define __PLAYERCONTROLLER_H__

#include "RuntimeInclude.h"
RUNTIME_MODIFIABLE_INCLUDE;
#include "raylib.h"
#include "platformerEngineBasics.h"

using namespace std;

class PlayerController : public Behaviour {
public:
    PlayerController() {
        // Don't delete this, the constructor is required
    }

    // /*
    void update() {
        // Code written here will be run every frame
        Vector2 dir{0.0f, 0.0f};
        if(IsKeyDown(KEY_W)) {
            _go->setYOffset(_go->getOffset().y - 300.0f * GetFrameTime());
            dir.y = -1;
        }
        if(IsKeyDown(KEY_S)) {
            _go->setYOffset(_go->getOffset().y + 300.0f * GetFrameTime());
            dir.y = 1;
        }
        if(IsKeyDown(KEY_A)) {
            _go->setXOffset(_go->getOffset().x - 300.0f * GetFrameTime());
            dir.x = 1;
        }
        if(IsKeyDown(KEY_D)) {
            _go->setXOffset(_go->getOffset().x + 300.0f * GetFrameTime());
            dir.x = -1;
        }
        if(auto j = _go->getBehaviour<Animator>()) {
            j->setValue("var1", dir.y);
            j->setValue("var2", dir.x);
        }
    }
    // */

    // /*
    void draw() {
        // Code written here will be run every frame after every update is run
    }
    // */
    // Do not delete from here
    shared_ptr<Behaviour> clone() const { return make_shared<PlayerController>(*this); }
    string dataToString() const { return "playercontroller:"; }
}; 

#endif
