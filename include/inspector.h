#ifndef __INSPECTOR_H__
#define __INSPECTOR_H__

#include "raylib.h"
#include "window.h"
#include "tracker.h"
#include "viewTransform.h"
#include "viewGameobject.h"
#include "viewAnimatedsprite.h"
#include "viewSprite.h"
#include "viewRigidbody.h"
#include "viewAABB.h"
#include "viewButton.h"
#include "viewLabel.h"
// #include "viewPaint.h"
#include "engineOptionList.h"
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
                make_shared<ViewSprite>(),
                make_shared<ViewRigidBody>(),
                make_shared<ViewAABB>(),
                make_shared<ViewButton>(),
                make_shared<ViewLabel>(bind(&Inspector::reload, this)),
                make_shared<ViewPaint>()
            };
            _buttonsize = Vector2{346, 30};
            _options = nullptr;
        }

        void update();
        void setObservers(shared_ptr<GameObject> go);
        void reload();

        void draw();

    private:
        Tracker _tracker;
        vector<shared_ptr<Observer>> _observerList;

        shared_ptr<GameObject> _go;
        Vector2 _buttonPosition, _buttonsize;

        shared_ptr<EngineOptionList> _options;

        void addSprite();
        void addAnimatedSprite();
        void addLabel();
        void addButton();
};

#endif