#ifndef __NEWBEHAVIOUR_H__
#define __NEWBEHAVIOUR_H__

RUNTIME_MODIFIABLE_INCLUDE; //recompile runtime files when this changes
#include "raylib.h"
#include "platformerEngineBasics.h"

using namespace std;

class NewBehaviour : public Behaviour {
private:
    // Declare variables here
public:
    NewBehaviour() {
        // Don't delete this, the constructor is required
    }

    // /*
    void update() {
        // Code written here will be run every frame
    }
    // */

    // /*
    void draw() {
        // Code written here will be run every frame after every update is run
    }
    // */
};

#endif
