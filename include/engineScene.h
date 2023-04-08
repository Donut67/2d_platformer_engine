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
            font = LoadFont("resources/monogram.ttf");
        }

        EngineScene(vector<string> fileNames, const int &act) : Scene(fileNames, act){
            // pre:  true
            // post: constructor from the set of creator files on 'fileNames' and sets the current scene
            font = LoadFont("resources/monogram.ttf");
        }

        shared_ptr<GameObject> setNewChildTo(const shared_ptr<GameObject> &go);

        shared_ptr<GameObject> getNthGameObject(const int& n);
        shared_ptr<GameObject> findReversePre(Vector2 position);

        void drawTreePostorder(const Vector2 &pos) const;

    private: 
        Font font;

        shared_ptr<GameObject> findReversePre_i(const NTree<GameObject> &a, Vector2 position, bool &found);
        shared_ptr<GameObject> getNthGameObject_i(const NTree<GameObject> &a, int &n, bool &found);

        void drawTreePostorder_i(const NTree<GameObject> &a, Vector2 &pos, const int &depth, const float &x) const; 

};

#endif