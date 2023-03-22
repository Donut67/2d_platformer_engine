#ifndef __AABB_BOX_H__
#define __AABB_BOX_H__

#include "raylib.h"
#include "gameObject.h"
#include "collision.h"
#include "nTree.h"
#include <cstdlib>
#include <iostream>
#include <memory>
#include <vector>

using namespace std;

class GameObject;
class Collision;

class AABB : public Collision{
    public:
        // CONSTRUCTORS
        AABB();
        AABB(bool s, bool t);

        // DESTRUCTORS

        // MODIFIERS
        void update();

        void setGameObject(shared_ptr<GameObject> go);

        // CONSULTORS
        Vector2 overlap(AABB ref) const;
        Vector2 getDirection() const;
        bool isColliding() const;

        bool getStatic() const;
        bool getTransparent() const;
        bool getActive() const;

        void draw();

    private:
        Vector2 _dir;
        bool _static, _transparent, _isActive;

        shared_ptr<GameObject> _gameObject;

        void getCollisions_i(const NTree<GameObject> &a, vector<AABB> &vec) const;
        vector<AABB> getCollisions(const NTree<GameObject> &a) const;

        Vector2 xAABB(const AABB &ref) const;
};
#endif