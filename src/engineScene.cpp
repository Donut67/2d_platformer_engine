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

shared_ptr<GameObject> EngineScene::findReversePre_i(const NTree<GameObject> &a, Vector2 position) {
    for (int i = a.nChilds() - 1; i >= 0; i--) {
        findReversePre_i(a.child(i), position);
    }
    std::cout << a.content()->name() << " ";

    return nullptr;
}

shared_ptr<GameObject> EngineScene::getNthGameObject_i(const NTree<GameObject> &a, int &n, bool &found){
    shared_ptr<GameObject> possible;

    if(!a.isEmpty() && a.content()->isEnable() && !a.content()->isPaused()){
        if(n == 0) {
            found = true;
            return a.content();
        }
        if(a.content()->WindowShouldClose()) _shouldClose = true;
        for(int i = 1; i <= a.nChilds(); i++) {
            n--;
            possible = getNthGameObject_i(a.child(i), n, found);
        }

        return possible;
    }
    return nullptr;
}