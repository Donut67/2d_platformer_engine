#ifndef __SETTINGS_CONTENT_H__
#define __SETTINGS_CONTENT_H__

#include "raylib.h"
#include "gameObject.h"
#include "monoBehavior.h"
#include <cstdlib>
#include <iostream>

using namespace std;

class GameObject;
class MonoBehavior;

class SettingsContent : public MonoBehavior{
    public:
        // CONSTRUCTORS
        SettingsContent();

        // DESTRUCTORS

        // MODIFIERS
        void setGameObject(shared_ptr<GameObject> go);

        void update();

        // CONSULTORS
        void draw();

    private:
        string _actual;

};
#endif