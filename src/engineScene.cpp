#include "../include/engineScene.h"
#include <cstdlib>
#include <iostream>

using namespace std;

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