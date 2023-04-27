#include "raylib.h"
#include "scene.h"
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <variant>
#include <vector>
#include <memory>

using namespace std;

void serializeMonoBehavior(string comp, shared_ptr<GameObject> object){
    if(comp == "playercontroller") object->addComponent(make_shared<PlayerController>(PlayerController()));
    else if(comp == "settingscontent") object->addComponent(make_shared<SettingsContent>(SettingsContent()));
    else if(comp == "settingsbuttonbehavior") object->addComponent(make_shared<SettingsButtonBehavior>(SettingsButtonBehavior()));
    else if(comp == "tabbuttonbehavior") object->addComponent(make_shared<TabButtonBehavior>(TabButtonBehavior()));
    else if(comp == "menucontroller") object->addComponent(make_shared<MenuController>(MenuController()));
}

string getNextWord(string &line, int &actual){
    int pos = 0, wordPos = 0;
    bool found;

    while(!found && pos != int(line.size())){
        found = line[pos] != ' ';
        if(!found) pos += 4;
    }

    if(found){
        found = false;
        while(!found && pos + wordPos != int(line.size())){
            found = line[pos + wordPos] == ':';
            if(!found) wordPos ++;
        }
        actual = pos + wordPos;
    } else actual = -1;

    string ret = line.substr(pos, wordPos);
    if(pos + wordPos + 2 <= (int)line.length()) line = line.substr(pos + wordPos + 2, line.length());
    else line = "";

    return ret;
}

NTree<GameObject> Scene::getNextGameObject(ifstream &file, bool &end){
    NTree<GameObject> result, child;
    string line, objectName, part, comp;
    int pos;
    bool ended = false;

    getline(file, line);
    objectName = getNextWord(line, pos);

    end = objectName == "end" || file.eof();
    
    if(!end){
        shared_ptr<GameObject> object = make_shared<GameObject>(GameObject(objectName, _rootlist.back()));
        getline(file, line);
        part = getNextWord(line, pos);

        while(!ended){
            if(part == "components"){
                bool endComp = false;
                while(!endComp){
                    getline(file, line);
                    comp = getNextWord(line, pos);
                    if(comp == "transform"){
                        vector<string> aux1_1 = splice(line, ',');
                        vector<string> aux2_1 = splice(aux1_1[0], ' ');
                        vector<string> aux2_2 = splice(aux1_1[1], ' ');

                        Vector2 position{stof(aux2_1[0]), stof(aux2_1[1])};
                        Vector2 scale{stof(aux2_2[0]), stof(aux2_2[1])};
                        
                        object->addComponent(make_shared<TransformComp>(position, scale));
                    }else if(comp == "tilemap"){
                        vector<string> aux1_1 = splice(line, ',');
                        vector<string> aux2_2 = splice(aux1_1[2], ' ');
                        
                        shared_ptr<Texture2D> tex;

                        if(_texturelist.find(aux1_1[0]) != _texturelist.end()){
                            tex = _texturelist.find(aux1_1[0])->second;
                        }else{
                            string str = "resources/" + aux1_1[0];
                            char *cstr = new char[str.length() + 1];
                            strcpy(cstr, str.c_str());
                            tex = make_shared<Texture2D>(LoadTexture(cstr));
                            _texturelist.insert(pair<string, shared_ptr<Texture2D>>(aux1_1[0], tex));
                        }

                        TileSet t = TileSet(tex, aux1_1[1]);
                        
                        Vector2 tileMapSize{stof(aux2_2[0]), stof(aux2_2[1])};
                        float tileMapScale = stof(aux1_1[3]);
                        string tileMapMeta = aux1_1[4];

                        object->addComponent(make_shared<TileMap>(TileMap(t, tileMapSize, tileMapScale, tileMapMeta)));
                    }else if(comp == "paint"){
                        vector<string> aux = splice(line, ' ');
                        Color color{stouc(aux[0]), stouc(aux[1]), stouc(aux[2]), stouc(aux[3])};

                        object->addComponent(make_shared<Paint>(Paint(color)));
                    }else if(comp == "animatedsprite"){
                        vector<string> aux1_1 = splice(line, ',');

                        shared_ptr<Texture2D> tex;

                        if(_texturelist.find(aux1_1[0]) != _texturelist.end()){
                            tex = _texturelist.find(aux1_1[0])->second;
                        }else{
                            string str = "resources/" + aux1_1[0];
                            char *cstr = new char[str.length() + 1];
                            strcpy(cstr, str.c_str());
                            tex = make_shared<Texture2D>(LoadTexture(cstr));
                            _texturelist.insert(pair<string, shared_ptr<Texture2D>>(aux1_1[0], tex));
                        }

                        float scale     = stof(aux1_1[1]);
                        string map_meta = aux1_1[2];
                        float timeFrame = stof(aux1_1[3]);

                        object->addComponent(make_shared<AnimatedSprite>(AnimatedSprite(aux1_1[0], tex, scale, map_meta, timeFrame)));
                    }else if(comp == "aabb"){
                        vector<string> aux1_1 = splice(line, ',');
                        
                        object->addComponent(make_shared<AABB>(AABB(aux1_1[0] == "true", aux1_1[1] == "true")));
                    }else if(comp == "button"){
                        vector<string> aux1 = splice(line, ',');

                        vector<string> aux2 = splice(aux1[0], ' ');
                        Color color{stouc(aux2[0]), stouc(aux2[1]), stouc(aux2[2]), stouc(aux2[3])};
                        if(aux1.size() > 1){
                            vector<string> aux3 = splice(aux1[1], ' ');
                            Color hover{stouc(aux3[0]), stouc(aux3[1]), stouc(aux3[2]), stouc(aux3[3])};
                            object->addComponent(make_shared<Button>(Button(color, hover)));
                        } else object->addComponent(make_shared<Button>(Button(color)));
                    }else if(comp == "label"){
                        vector<string> aux = splice(line, ',');

                        shared_ptr<Font> font;

                        if(_fontlist.find(aux[1]) != _fontlist.end()){
                            font = _fontlist.find(aux[1])->second;
                        }else{
                            string str = "resources/" + aux[1];
                            char *cstr = new char[str.length() + 1];
                            strcpy(cstr, str.c_str());
                            font = make_shared<Font>(LoadFont(cstr));
                            _fontlist.insert(pair<string, shared_ptr<Font>>(aux[1], font));
                        }

                        string text = aux[0];
                        char allign = aux[2].c_str()[0];
                        int fontSize = stoi(aux[3]);
                        vector<string> aux1_2 = splice(aux[4], ' ');
                        Color color{stouc(aux1_2[0]), stouc(aux1_2[1]), stouc(aux1_2[2]), stouc(aux1_2[3])};

                        object->addComponent(make_shared<Label>(Label(text, font, allign, fontSize, color)));
                    }else if(comp == "camera"){
                        object->addComponent(make_shared<TrackCamera>(TrackCamera()));
                    }else if(comp == "mosaic"){
                        vector<string> aux1 = splice(line, ',');shared_ptr<Texture2D> tex;

                        if(_texturelist.find(aux1[0]) != _texturelist.end()){
                            tex = _texturelist.find(aux1[0])->second;
                        }else{
                            string str = "resources/" + aux1[0];
                            char *cstr = new char[str.length() + 1];
                            strcpy(cstr, str.c_str());
                            tex = make_shared<Texture2D>(LoadTexture(cstr));
                            _texturelist.insert(pair<string, shared_ptr<Texture2D>>(aux1[0], tex));
                        }

                        object->addComponent(make_shared<Mosaic>(Mosaic(tex, stof(aux1[1]))));
                    }else if(comp == "sprite"){
                        vector<string> aux1 = splice(line, ',');shared_ptr<Texture2D> tex;

                        if(_texturelist.find(aux1[0]) != _texturelist.end()){
                            tex = _texturelist.find(aux1[0])->second;
                        }else{
                            string str = "resources/" + aux1[0];
                            char *cstr = new char[str.length() + 1];
                            strcpy(cstr, str.c_str());
                            tex = make_shared<Texture2D>(LoadTexture(cstr));
                            _texturelist.insert(pair<string, shared_ptr<Texture2D>>(aux1[0], tex));
                        }

                        object->addComponent(make_shared<Sprite>(Sprite(tex, stof(aux1[1]))));
                    }else if(comp == "animation"){
                        object->addComponent(make_shared<Animation>(Animation(line)));
                    }else if(comp == "rigidbody"){
                        object->addComponent(make_shared<RigidBody>(RigidBody()));
                    }else if(comp == "selector"){
                        object->addComponent(make_shared<Selector>(Selector()));
                    }else if(comp == "end") endComp = true;
                    else serializeMonoBehavior(comp, object);
                }
            }else if(part == "objects") {
                bool endObj = false;
                while(!endObj) {
                    NTree<GameObject> aux = getNextGameObject(file, endObj);
                    if(!endObj) child.setBrother(aux);
                }
            }

            getline(file, line);
            part = getNextWord(line, pos);
            ended = part == "end";
        }
        // object->setScene(shared_from_this());
        result.setBrother(NTree<GameObject>(child, object));
    }

    return result;
}

// CONSTRUCTORS
Scene::Scene(){
    _shouldClose = false;
    _actual = -1;
}

Scene::Scene(vector<string> fileName, const int &act){
    for(auto i : fileName){
        shared_ptr<NTree<GameObject>> root = shared_ptr<NTree<GameObject>>(new NTree<GameObject>);
        _rootlist.push_back(root);
        bool ended = false;

        ifstream file("resources/" + i);
        
        while(!ended) {
            NTree<GameObject> aux = getNextGameObject(file, ended);
            if(!ended) root->setBrother(aux);
        }

        file.close();
    }
    
    _shouldClose = false;
    _actual = act;
}
// DESTRUCTORS

// MODIFIERS
void Scene::update(){
    for(int i = 1; i <= _rootlist[_actual]->nBrothers(); i++) updatePostorder(_rootlist[_actual]->brother(i));
}

void Scene::changeScene(const int &scene){
    _actual = scene;
}

// CONSULTORS
int Scene::changeScene() const{
    return _actual;
}

bool Scene::WindowShouldClose() const{
    return _shouldClose;
}

NTree<GameObject> Scene::getTree() const{
    return *_rootlist[_actual];
}

void Scene::draw(){
    for(int i = 1; i <= _rootlist[_actual]->nBrothers(); i++) drawPostorder(_rootlist[_actual]->brother(i));
}

bool Scene::exists(const NTree<GameObject> &a, string name){
    bool found = false;
    if(!a.isEmpty()){
        if(a.content()->name() == name) found = true;
        else {
            int i = 1;
            while(!found && i <= a.nChilds()){
                found = exists(a.child(i), name);
                if(!found) i++;
            }
        }
    }
    return found;
}

void Scene::updatePostorder(const NTree<GameObject> &a){
// pre:  true
// post: updates all the elements of the tree
    if(!a.isEmpty() && a.content()->isEnable() && !a.content()->isPaused()){
        a.content()->update();
        if(a.content()->WindowShouldClose()) _shouldClose = true;
        for(int i = 1; i <= a.nChilds(); i++) updatePostorder(a.child(i));
    }
}

void Scene::drawPostorder(const NTree<GameObject> &a){
// pre:  true
// post: draws all the elements of the tree
    if(!a.isEmpty() && a.content()->isEnable()){
        a.content()->draw();
        for(int i = 1; i <= a.nChilds(); i++) drawPostorder(a.child(i));
    }
}
