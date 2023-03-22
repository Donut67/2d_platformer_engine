#include "../include/raylib.h"
#include "../include/AABB.h"
#include <cstdlib>
#include <iostream>
#include <string>

using namespace std;

void AABB::getCollisions_i(const NTree<GameObject> &a, vector<AABB> &vec) const{
    if(!a.isEmpty()){
        for(int i = 1; i <= a.nChilds(); i++) getCollisions_i(a.child(i), vec);
        for(auto i : a.content()->getComponents()) if(auto item = dynamic_pointer_cast<AABB>(i)) vec.push_back(*item);
    }
}

vector<AABB> AABB::getCollisions(const NTree<GameObject> &a) const{
    vector<AABB> vec;

    getCollisions_i(a, vec);

    return vec;
}


AABB::AABB(){
    _static = true;
    _transparent = _isActive = false;
    _dir = Vector2{0.0f, 0.0f};
}

AABB::AABB(bool s, bool t){
    _static = s;
    _transparent = t;
    _isActive = false;
    _dir = Vector2{0.0f, 0.0f};
}

void AABB::update(){
    _isActive = false;
    if(!_static){
        vector<AABB> aabb = getCollisions(*_gameObject->getRoot());
        vector<AABB> collision;
        Vector2 reposition = Vector2{0.0f, 0.0f};

        for(AABB i : aabb){
            if(i._gameObject->name() != _gameObject->name()){
                Vector2 tmp = overlap(i);
                reposition = Vector2{abs(tmp.x) > abs(reposition.x)? tmp.x : reposition.x, abs(tmp.y) > abs(reposition.y)? tmp.y : reposition.y};
            }
        }

        if(reposition.x != 0.0f || reposition.y != 0.0f) _isActive = true;
        // if(!_transparent) {
        //     _dir = reposition;
        //     Vector2 pos = (*_gameObject->getParent().content())[TransformComp()]->getPosition();
        //     (*_gameObject->getParent().content())[TransformComp()]->setPosition(Vector2{pos.x + reposition.x, pos.y + reposition.y});
        // }
    }
}

void AABB::setGameObject(shared_ptr<GameObject> go){
    _gameObject = go;
}

Vector2 AABB::overlap(AABB ref) const{
    Vector2 repos   = Vector2{0.0f, 0.0f};
    
    repos = xAABB(ref);

    return repos;
}

Vector2 AABB::getDirection() const{
    return _dir;
}

bool AABB::isColliding() const{
    return _dir.x != 0 || _dir.y != 0;
}

bool AABB::getActive() const{
    return _isActive;
}

bool AABB::getStatic() const{
    return _static;
}

bool AABB::getTransparent() const{
    return _transparent;
}

void AABB::draw(){
    Vector2 pos  = (*_gameObject)[TransformComp()]->getGlobalPosition();
    Vector2 size = (*_gameObject)[TransformComp()]->getScale();
    DrawRectangle(pos.x, pos.y, size.x, size.y, LIME);
}

Vector2 AABB::xAABB(const AABB &ref) const{
    Vector2 repos   = Vector2{0.0f, 0.0f};
    Vector2 pos     = (*_gameObject)[TransformComp()]->getGlobalPosition();
    Vector2 refPos  = (*ref._gameObject)[TransformComp()]->getGlobalPosition();
    Vector2 size    = (*_gameObject)[TransformComp()]->getScale();
    Vector2 refsize = (*ref._gameObject)[TransformComp()]->getScale();

    Rectangle thisrect = Rectangle{pos.x, pos.y, size.x, size.y};
    Rectangle rec = Rectangle{refPos.x, refPos.y, refsize.x, refsize.y};

    Rectangle col = GetCollisionRec(thisrect, rec);

    if(CheckCollisionRecs(thisrect, rec)) {
        if(col.height < col.width){
            Rectangle new_col = thisrect;
            float new_y = new_col.y;
            if(thisrect.y - rec.y >= 0) new_y = col.height;
            else new_y = -col.height;
            new_col.y += new_y;

            if(CheckCollisionRecs(new_col, rec)){
                new_col = thisrect;
                float new_x = new_col.x;
                if(thisrect.x - rec.x >= 0) new_x = col.width;
                else new_x = -col.width;
                new_col.x += new_x;

                repos.x = abs(new_x) > abs(repos.x)? new_x : repos.x;
            } else repos.y = abs(new_y) > abs(repos.y)? new_y : repos.y;
        }else{
            Rectangle new_col = thisrect;
            float new_x = new_col.x;
            if(thisrect.x - rec.x >= 0) new_x = col.width;
            else new_x = -col.width;
            new_col.x += new_x;

            if(CheckCollisionRecs(new_col, rec)){
                new_col = thisrect;
                float new_y = new_col.y;
                if(thisrect.y - rec.y >= 0) new_y = col.height;
                else new_y = -col.height;
                new_col.y += new_y;

                repos.y   = new_y;
            }else repos.x = new_x;
        }
    }

    return repos;
}