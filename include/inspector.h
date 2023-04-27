#ifndef __INSPECTOR_H__
#define __INSPECTOR_H__

#include "raylib.h"
#include "window.h"
#include "tracker.h"
#include "viewTransform.h"
#include "viewGameobject.h"
#include "viewAnimatedsprite.h"
#include "viewRigidbody.h"
#include "viewAABB.h"
#include <cstdlib>
#include <iostream>

using namespace std;

class Inspector : public Window {
    public:
        Inspector(Vector2 pos) : Window("Inspector", pos, Vector2{450, 800}){
            _observerList = vector<shared_ptr<Observer>>{
                make_shared<ViewGameobject>(),
                make_shared<ViewTransform>(),
                make_shared<ViewAnimatedSprite>(),
                make_shared<ViewRigidBody>(),
                make_shared<ViewAABB>()
            };
        }

        void update();
        void setObservers(shared_ptr<GameObject> go);

        void draw();

    private:
        Tracker _tracker;
        vector<shared_ptr<Observer>> _observerList;
};

#endif