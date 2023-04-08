#include "../include/engineScene.h"
#include <cstdlib>
#include <iostream>

using namespace std;

shared_ptr<GameObject> EngineScene::setNewChildTo(const shared_ptr<GameObject> &go){
    shared_ptr<GameObject> object = make_shared<GameObject>(GameObject("newGameObject", _rootlist.back()));
    object->addComponent(make_shared<TransformComp>(Vector2{0.0f, 0.0f}, Vector2{10.0f, 10.0f}));
    
    if(go != nullptr) _rootlist[_actual]->find(go->name()).child(0).setBrother(NTree<GameObject>(NTree<GameObject>(), object));
    else _rootlist[_actual]->setBrother(NTree<GameObject>(NTree<GameObject>(), object));

    return object;
}

shared_ptr<GameObject> EngineScene::getNthGameObject(const int& n){
    shared_ptr<GameObject> possible;
    bool found = false;
    int depth = n;
    int i = 1;

    while( i <= _rootlist[_actual]->nBrothers() && !found) {
        possible = getNthGameObject_i(_rootlist[_actual]->brother(i), depth, found);
        depth--;
        i++;
    }

    return found? possible : nullptr;
}

shared_ptr<GameObject> EngineScene::findReversePre(Vector2 position) {
    shared_ptr<GameObject> possible;

    bool found = false;
    int i = _rootlist[_actual]->nBrothers();

    while( i > 0 && !found) {
        possible = findReversePre_i(_rootlist[_actual]->brother(i), position, found);
        i--;
    }

    return possible;
}

void EngineScene::drawTreePostorder(const Vector2 &pos) const {
    Vector2 position = pos;
    for(int i = 1; i <= _rootlist[_actual]->nBrothers(); i++) drawTreePostorder_i(_rootlist[_actual]->brother(i), position, 0, pos.x);
}

shared_ptr<GameObject> EngineScene::findReversePre_i(const NTree<GameObject> &a, Vector2 position, bool &found) {
    shared_ptr<GameObject> possible = nullptr;
    
    if(!a.isEmpty() && a.content()->isEnable() && !a.content()->isPaused()){
        int i = a.nChilds();
        while ( i > 0 && !found ) {
            possible = findReversePre_i(a.child(i), position, found);
            i--;
        }

        if(!found) {
            Vector2 pos  = (*a.content())[TransformComp()]->getGlobalPosition();
            Vector2 size = (*a.content())[TransformComp()]->getSize();

            if(CheckCollisionPointRec(position, Rectangle{pos.x, pos.y, size.x, size.y})) {
                found = true;
                return a.content();
            }   
        }
    }

    return possible;
}

shared_ptr<GameObject> EngineScene::getNthGameObject_i(const NTree<GameObject> &a, int &n, bool &found){
    shared_ptr<GameObject> possible = nullptr;

    if(!a.isEmpty() && a.content()->isEnable() && !a.content()->isPaused()){
        if(n == 0) {
            found = true;
            return a.content();
        }
        if(a.content()->WindowShouldClose()) _shouldClose = true;

        int i = 0;
        while( i <= _rootlist[_actual]->nBrothers() && !found) {
            n--;
            possible = getNthGameObject_i(a.child(i), n, found);
            i++;
        }
    }

    return possible;
}

void EngineScene::drawTreePostorder_i(const NTree<GameObject> &a, Vector2 &pos, const int &depth, const float &x) const {
    if(!a.isEmpty()){
        DrawRectangle(x, pos.y, 300, 20, CLITERAL(Color){30, 30, 30, 255});
        DrawTextEx(font, a.content()->name().c_str(), Vector2{x + depth + 5, pos.y}, 18, 0, WHITE);
        pos.y += 20;
        for(int i = 1; i <= a.nChilds(); i++) drawTreePostorder_i(a.child(i), pos, depth + 10, x);
    }
}