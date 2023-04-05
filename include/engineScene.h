#ifndef __ENGINE_SCENE_H__
#define __ENGINE_SCENE_H__

#include "scene.h"
#include <cstdlib>
#include <iostream>
#include <memory>
#include <vector>

using namespace std;

class EngineScene : public Scene {
    public:
        // [[ CONSTRUCTORS ]]
        EngineScene() : Scene(){
            // pre:  true
            // post: default constructor

        }

        EngineScene(vector<string> fileNames, const int &act) : Scene(fileNames, act){
            // pre:  true
            // post: constructor from the set of creator files on 'fileNames' and sets the current scene

        }

        shared_ptr<GameObject> getNthGameObject(const int& n);
        shared_ptr<GameObject> findReversePre(Vector2 position);

    private: 

        shared_ptr<GameObject> findReversePre_i(const NTree<GameObject> &a, Vector2 position, bool &found);
        shared_ptr<GameObject> getNthGameObject_i(const NTree<GameObject> &a, int &n, bool &found);

};

#endif