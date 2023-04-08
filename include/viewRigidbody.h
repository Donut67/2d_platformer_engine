#ifndef __VIEW_RIGIDBODY_H__
#define __VIEW_RIGIDBODY_H__

#include "viewer.h"
#include "subject.h"
#include "raylib.h"
// #include "transform.h"

class ViewRigidBody : public Viewer, public Observer {
    private:
        Font _font;
        
        Vector2 _velocity;
        float _top;

    public: 
        ViewRigidBody() {
            _font = LoadFont("resources/monogram.ttf");
        }
        void show();
        void update(const GameObject& go, const float &height);
};
#endif