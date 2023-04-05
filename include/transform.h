#ifndef __TRANSFORM_H__
#define __TRANSFORM_H__

#include "raylib.h"
#include "gameObject.h"
#include "component.h"
#include <cstdlib>
#include <iostream>
#include <memory>

using namespace std;

class GameObject;
class Component;

class TransformComp : public Component{
    public:
        // CONSTRUCTORS
        TransformComp();
        TransformComp(Vector2 pos, Vector2 scale);

        // DESTRUCTORS

        // MODIFIERS
        void update();
        void setPosition(Vector2 pos);
        void setScale(Vector2 scale);

        void setGameObject(shared_ptr<GameObject> go);

        // CONSULTORS
        Vector2 getPosition() const;
        Vector2 getGlobalPosition() const;
        Vector2 getScale() const;
        Vector2 getSize() const;

        void draw();

    private:
        Vector2 _position, _scale;

        shared_ptr<GameObject> _gameObject;

        void findThis_i(const NTree<GameObject> &a, Vector2 &vec, bool &ended) const;
        Vector2 findThis() const;
};
#endif