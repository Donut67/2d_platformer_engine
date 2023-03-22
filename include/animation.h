#ifndef __ANIMATION_TRACK_H__
#define __ANIMATION_TRACK_H__

#include "raylib.h"
#include "gameObject.h"
#include "component.h"
#include <cstdlib>
#include <iostream>
#include <memory>

using namespace std;

class GameObject;
class Component;

typedef struct animationTrack{
    float time;
    string type, conmponent, part;
    float initial, ending;
} animationTrack;

class Animation : public Component{
    public:
        // CONSTRUCTORS
        Animation();
        Animation(string meta);

        // DESTRUCTORS
        ~Animation();

        // MODIFIERS
        void setGameObject(shared_ptr<GameObject> go);

        void start();
        void restart();
        void stop();

        void update();

        // CONSULTORS
        bool isStarted() const;
        bool isEnded() const;
        
        void draw();

    private:
        vector<animationTrack> _tracks;
        int _actual;
        float _intime;
        bool _started, _ended;

        shared_ptr<GameObject> _gameObject;

        float solveSin(float ini, float end, float time, float act);
        float solveRamp(float ini, float end, float time, float act);

};
#endif