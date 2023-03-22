#ifndef __PLAYER_CONTROLLER_H__
#define __PLAYER_CONTROLLER_H__

#include "raylib.h"
#include "gameObject.h"
#include "monoBehavior.h"
#include <cstdlib>
#include <iostream>

using namespace std;

class GameObject;
class MonoBehavior;

class PlayerController : public MonoBehavior{
    public:
        // CONSTRUCTORS
        PlayerController();

        // DESTRUCTORS

        // MODIFIERS
        void setGameObject(shared_ptr<GameObject> go);

        void update();

        // CONSULTORS
        float getStartJump() const;
        Vector2 getSpeed() const;

        void draw();

    private:
        bool _jumping, _falling, _wallSlide, _jumpOnWallSlide, _bottom, _back, _front, _top, _animation;
        float _startJump, _jumpOnWallSlideCoolDown, _movementSpeed;
        Vector2 _speed;

        shared_ptr<GameObject> _gameObject;


};
#endif