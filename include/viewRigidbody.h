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
        Vector2 _pos;
        shared_ptr<GameObject> _go;

    public: 
        ViewRigidBody() {
            _font = LoadFont("resources/monogram.ttf");
        }
        void show();
        void update(shared_ptr<GameObject> go, const Vector2 &pos);
};
#endif