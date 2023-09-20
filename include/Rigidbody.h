#ifndef __RIGIDBODY_H__
#define __RIGIDBODY_H__

#include "RuntimeSourceDependency.h"
RUNTIME_MODIFIABLE_INCLUDE; //recompile runtime files when this changes
#include "raylib.h"
#include "platformerEngineBasics.h"

using namespace std;

class Rigidbody : public Behaviour {
private:
    // Declare variables here
public:
    Rigidbody() {
        // Don't delete this, the constructor is required
    }

    // /*
    void update() {
        // Code written here will be run every frame
        _go->setYOffset(_go->getOffset().y + 10.0f);
    }
    // */

    // /*
    void draw() {
        // Code written here will be run every frame after every update is run
    }
    // */
    // Do not delete from here
    shared_ptr<Behaviour> clone() const { return make_shared<Rigidbody>(*this); }
    string dataToString() const { return "rigidbody:"; }
}; 

#endif
