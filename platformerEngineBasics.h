#ifndef __PLATFORMER_ENGINE_BASICS_H__
#define __PLATFORMER_ENGINE_BASICS_H__

/*
#include "RuntimeInclude.h"
RUNTIME_MODIFIABLE_INCLUDE;
/*
#include "RuntimeSourceDependency.h"
RUNTIME_COMPILER_SOURCEDEPENDENCY;
// */

#include "raylib.h"
#include "Nary.h"
#include "Graph.h"

#include <iostream>
#include <memory>
#include <vector>
#include <map>
#include <string>
#include <cstring>
#include <functional>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <cstdlib>
#include <variant>
#include <math.h>
#include <filesystem>

using namespace std;

string getNextWord(string &line, int &actual);
string ftostr(float f, int precision);
unsigned char stouc(const string &source);
string slice(string source, char divider, int &pos);
vector<string> splice(string source, char divider);
string str_toupper(string s);
string str_tolower(string s);

class TileSetData;
class EngineAnimation;
class EngineAnimationGraph;

class Resources {
    private:
        static shared_ptr<Resources> _instance;
        map<string, shared_ptr<Texture2D>> _texture_list;
        map<string, shared_ptr<Font>> _font_list;
        map<string, shared_ptr<TileSetData>> _tileset_list;
        map<string, shared_ptr<EngineAnimation>> _animation_list;
        // map<string, shared_ptr<EngineAnimationGraph>> _animation_graph_list;

        void addTexture(const string &filename);
        void addFont(const string &filename);
        void addTileset(const string& filename);
        void addAnimation(const string& filename);
        // void addAnimationGraph(const string& filename);
    protected:
    public:
        Resources() {}
        static shared_ptr<Resources> getInstance();

        shared_ptr<Texture2D> texture(const string &filename, const bool &load = true);
        shared_ptr<Font> font(const string &filename, const bool &load = true);
        shared_ptr<TileSetData> tileset(const string& filename, const bool& load = true);
        shared_ptr<EngineAnimation> animation(const string& filename, const bool& load = true);
        // shared_ptr<EngineAnimationGraph> animation_graph(const string& filename, const bool& load);
};

class TileSetData {
    public:
        string _path;
        shared_ptr<Texture2D> _texture;
        Vector2 _tileSize;
        Vector2 _nTiles;

        TileSetData() {
            _tileSize = Vector2{0.0f, 0.0f};
            _nTiles = Vector2{0.0f, 0.0f};
            _texture = nullptr;
            _path = "";
        }
        TileSetData(string path) {
            string line;

            ifstream file(path);
            getline(file, line);

            vector<string> aux = splice(line, ' ');
            _path     = aux[0];
            _texture  = Resources::getInstance()->texture(_path, true);
            _nTiles   = Vector2{stof(aux[1]), stof(aux[2])};
            _tileSize = Vector2{stof(aux[3]), stof(aux[4])};
        }

        TileSetData& operator=(const TileSetData& d) { return *this; }

        void setTexture(string path) {
            _path     = path;
            _texture  = Resources::getInstance()->texture(_path, true);
            _nTiles   = { 1 };
            _tileSize = { 0 };
        }

        void setRows(int i) {
            _nTiles.y = (float)i;
            _tileSize.y = _texture->height / _nTiles.y;
        }
        void setColumns(int i) {
            _nTiles.x = (float)i;
            _tileSize.x = _texture->width / _nTiles.x;
        }
        Vector2 operator [](const int& pos) const {
            Vector2 res{ 0 };
            res.x = int(pos % (int)_nTiles.x);
            res.y = int(pos / (int)_nTiles.x);
            return res;
        }
        void save() {
            string data = _path + " " + to_string((int)_nTiles.x) + " " + to_string((int)_nTiles.y) + " " + to_string((int)_tileSize.x) + " " + to_string((int)_tileSize.y);
            string extension = ".tsd";

            ofstream file(_path.substr(0, _path.find('.')) + extension);
            file << data;
            file.close();
        }
};

class EngineAnimation{
public:
    string _name, _path;
    vector<pair<int, float>> _sequence;
    bool _loop;
    int _actual;
    float _frame_time;
    shared_ptr<TileSetData> _data;

    EngineAnimation() {
        _name = "New Animation";
        _loop = true;
        _actual = 0;
        _frame_time = 0.0f;
        _data = make_shared<TileSetData>();
        _path = "";
    }
    EngineAnimation(shared_ptr<TileSetData> data) {
        _name = "New Animation";
        _loop = true;
        _actual = 0;
        _frame_time = 0.0f;
        _data = data;
        _path = "";
    }
    EngineAnimation(string path) {
        _path = path;
        string line;
        vector<string> edges;

        ifstream file(path);
        getline(file, line);

        vector<string> aux = splice(line, ';');
        _name = aux[0];
        _data = Resources::getInstance()->tileset(aux[1], true);
        aux.erase(aux.begin());
        aux.erase(aux.begin());
        for (auto i : aux) {
            vector<string> aux1 = splice(i, ':');
            addPoint(stoi(aux1[0]), stof(aux1[1]));
        }
        _actual = 0;
    }
    EngineAnimation(string name, vector<pair<int, float>> sequence, bool loop, int actual, float frame_time) {
        _name = name;
        _sequence = sequence;
        _loop = loop;
        _actual = actual;
        _frame_time = frame_time;
        _data = nullptr;
    }

    void update() {
        if (_data->_texture != nullptr) {
            _frame_time -= GetFrameTime();

            if(_frame_time <= 0){
                _frame_time = _sequence[_actual].second;
                _actual ++;
            }

            bool ended = _actual > (int)_sequence.size() - 1;

            if(_loop && ended) _actual = 0;
            else if(!_loop && ended) _actual = (int)_sequence.size() - 1; 
        }
    }

    void draw(Vector2 pos, Vector2 size, bool _fliph, bool _flipv) {
        if (_data->_texture != nullptr) {
            int i = _sequence[_actual].first % (int)(_data->_nTiles.x), j = (int)floor(_sequence[_actual].first / (int)(_data->_nTiles.x));
            Rectangle src = Rectangle{
                i * _data->_tileSize.x,
                j * _data->_tileSize.y,
                (_fliph ? -_data->_tileSize.x : _data->_tileSize.x),
                (_flipv ? -_data->_tileSize.y : _data->_tileSize.y) };
            Rectangle dst = Rectangle{ pos.x, pos.y, size.x, size.y };
            DrawTexturePro(*_data->_texture, src, dst, Vector2{ 0.0f, 0.0f }, 0.0f, WHITE);
        }
    }

    void setTime(int pos, float time) {
        _sequence[pos].second = time;
    }

    void setTexture(shared_ptr<Texture2D> texture, string meta) {
        _data->_texture = texture;

        string line;
        ifstream file(meta);
        getline(file, line);
        _data->_nTiles = Vector2{stof(line.substr(0, line.find(','))), stof(line.substr(line.find(',') + 1, line.size()))};

        _data->_tileSize = Vector2{_data->_texture->width / _data->_nTiles.x, _data->_texture->height / _data->_nTiles.y};
    }

    void addPoint(int val, float time) {
        _sequence.push_back({val, time});
    }

    void save() {
        _path = _data->_path;
        string data = _name + ";" + _path.substr(0, _path.find('.')) + ".tsd";
        for (auto i : _sequence) data += ";" + to_string(i.first) + ":" + ftostr(i.second, 3);

        ofstream file(_path.substr(0, _path.find('.')) + _name + ".aon");
        file << data;
        file.close();
    }
};

typedef struct EngineAnimationGraph {
    string name, path;
    LabeledGraph<string, string> graph;
    map<string, shared_ptr<EngineAnimation>> _animations;
    map<string, int> _variables;

    EngineAnimationGraph() { 
        path = "resources/NewAnimationGraph1.aph";
    }
    EngineAnimationGraph(string p) {
        path = p;
        string line;
        vector<string> edges;

        ifstream file(path);
        getline(file, line);
        while (line != "end") {
            vector<string> aux = splice(line, ';');
            _animations[aux[0]] = Resources::getInstance()->animation(aux[1], true);
            getline(file, line);
        }
        getline(file, line);
        vector<string> aux = splice(line, ' ');
        for(auto i : aux) _variables[i] = 0;

        getline(file, line);
        while (!file.eof()) {
            vector<string> aux = splice(line, ';');
            string v = aux[0];
            graph.AddVertex(v);
            aux.erase(aux.begin());
            for (auto i : aux) {
                vector<string> aux1 = splice(i, ':');
                edges.push_back(v + ';' + aux1[0] + ';' + aux1[1]);
            }
            getline(file, line);
        }

        for (auto i : edges) {
            vector<string> aux = splice(i, ';');
            graph.AddEgde(aux[0], aux[2], aux[1]);
        }
    }

    void save(string p = "") {
        if (p != "") path = p;
        ofstream file(path);

        for (auto i : _animations) file << i.first << ';' << i.second->_path << '\n';
        file << "end\n";

        auto it = _variables.begin();
        while (it != _variables.end()) {
            if (it == --_variables.end()) file << it->first;
            else file << it->first << ' ';
            it++;
        }
        file << "\n";

        for (auto i : graph.GetVertex()) {
            string data = i;
            for (auto j : graph.GetEdges(i)) data += ';' + j + ':' + graph.GetNextVertex(i, j);
            file << data << '\n';
        }
        file.close();
    }

} EngineAnimationGraph;

class ObjectID {
    private:
        string _value;
    public:
        ObjectID(bool n);
        
        bool operator ==(const ObjectID & id);
        bool operator >(const ObjectID & id);
        
        void show() const;
};

class Object {
    protected:
    public:
        ObjectID _id; 
        Vector2 _position, _offset ,_size; 

        Object();
        Object(Vector2 offset, Vector2 size);
        
        void update(const Vector2 &offset);
        
        virtual void update() = 0;
        virtual void draw() = 0;
};

class GameObject;

class Behaviour {
    protected:
        shared_ptr<GameObject> _go = nullptr;
    public:
        virtual void update() {}
        virtual void draw() {}

        virtual void setGameObject(shared_ptr<GameObject> go);
        virtual string dataToString() { 
            cout << "base behavior\n";
            return ""; 
        }

        virtual std::shared_ptr<Behaviour> clone() const {
            return std::make_shared<Behaviour>(*this);
        }
};

class Label : public Behaviour{
    private:
        string _text;
        Font _font;
        char _text_allign;
        int _font_size;
        Color _text_color;
    public:
        Label() {
            _text = "New Label";
            _font_size = 18;
            _text_allign = 'l';
            _font = *Resources::getInstance()->font("resources/monogram.ttf", true);
            _text_color = WHITE;
        }
        Label(string line) {
            vector<string> aux = splice(line, ',');

            _text = aux[0];
            _font_size = stoi(aux[1]);
            _text_allign = aux[2].c_str()[0];

            vector<string> aux1_2 = splice(aux[3], ' ');
            _text_color = { stouc(aux1_2[0]), stouc(aux1_2[1]), stouc(aux1_2[2]), stouc(aux1_2[3]) };

            _font = *Resources::getInstance()->font("resources/monogram.ttf", true);
        }
        Label(string text, int font_size) : _text(text), _font_size(font_size){
            _text_allign = 'l';
            _font = *Resources::getInstance()->font("resources/monogram.ttf", true);
            _text_color = WHITE;
        }

        void setText(const string &text) {_text = text;}
        void setAllign(const char &c) {_text_allign = c;}
        void setFontSize(const int &s) {_font_size = s;}
        void setColor(const Color &c) {_text_color = c;}

        void draw();

        string text() const {return _text;}
        char text_allign() const {return _text_allign;}
        int font_size() const {return _font_size;}
        Color color() const {return _text_color;}

        string dataToString() {
            string data = "label: " + _text + ',' + to_string(_font_size) + ',' + _text_allign + ',' + to_string(_text_color.r) + ' ' + to_string(_text_color.g) + ' ' + to_string(_text_color.b) + ' ' + to_string(_text_color.a);
            return data;
        }
        std::shared_ptr<Behaviour> clone() const {
            return std::make_shared<Label>(*this);
        }
};

class Sprite : public Behaviour {
    private:
        string _filename;
        shared_ptr<Texture2D> _texture;
        Color _color;
    public:
        Sprite() : _color(WHITE) {}
        Sprite(string line) {
            vector<string> aux = splice(line, ',');
            vector<string> aux1_2 = splice(aux[0], ' ');

            if ((int)aux.size() == 2) {
                _texture = Resources::getInstance()->texture(aux[1], true);
                _filename = aux[1];
            }
            _color = { stouc(aux1_2[0]), stouc(aux1_2[1]), stouc(aux1_2[2]), stouc(aux1_2[3]) };
        }
        Sprite(Color color) : _color(color) {}
        Sprite(string filename, shared_ptr<Texture2D> texture, Color color) : _filename(filename), _texture(texture), _color(color) {}
        
        void setfilename(const string &name) {_filename = name;}
        void settexture(shared_ptr<Texture2D> text) {_texture = text;}
        void setcolor(const Color &c) {_color = c;}

        void draw();
        
        string filename() const {return _filename;}
        shared_ptr<Texture2D> texture() const {return _texture;}
        Color color() const {return _color;}

        string dataToString() {
            string data = "sprite: " + to_string(_color.r) + ' ' + to_string(_color.g) + ' ' + to_string(_color.b) + ' ' + to_string(_color.a);
            if (_filename != "") data += ',' + _filename;
            return data;
        }
        std::shared_ptr<Behaviour> clone() const {
            return std::make_shared<Sprite>(*this);
        }
};

class AnimatedSprite : public Behaviour{
    private:
        string _filename;
        shared_ptr<EngineAnimation> _animation;

        bool _fliph, _flipv;

    public:
        AnimatedSprite() { 
            _animation = nullptr;
            _filename = "";
            _fliph = _flipv = false;
        }
        AnimatedSprite(string line) {
            vector<string> aux = splice(line, ',');
                                                                                                          //  0  1  2  3
            _filename = aux.size() > 1? aux[0] : line;                                                    // 00 01 10 11
            _fliph = aux.size() > 1 ? stoi(aux[1]) > 1 : false;                                           // -- -v h- hv
            _flipv = aux.size() > 1 ? stoi(aux[1]) % 2 != 0 : false;

            _animation = Resources::getInstance()->animation(_filename, true);
        }

        void update() { if (_animation) _animation->update(); }
        void draw();

        string filename() const { return _filename; }
        bool fliph() const { return _fliph; }
        bool flipv() const { return _flipv; }

        void fliph(bool a) { _fliph = a; }
        void flipv(bool a) { _flipv = a; }
        void setAnimation(string filename) {
            _filename = filename;
            _animation = Resources::getInstance()->animation(_filename, true);
        }

        string dataToString() {
            int flip = 0;
            if (_fliph) flip += 2;
            if (_flipv) flip += 1;
            string data = "animatedsprite: " + _filename + "," + to_string(flip);
            return data;
        }
        std::shared_ptr<Behaviour> clone() const {
            return std::make_shared<AnimatedSprite>(*this);
        }
};

class TileMap : public Behaviour {
    private:
        string _filename;
        shared_ptr<TileSetData> _set;
        map<pair<int, int>, vector<int>> _tile_map;
        float _scale;
        Vector2 actual, _map_size;

        void construct(string set, float scale, string mapMeta) {
            _scale = scale;
            _set = Resources::getInstance()->tileset(set, true);
            _filename = mapMeta;
            _map_size = { 24.0f, 24.0f };
            actual = { 0.0f, 0.0f };

            if(mapMeta != "") {
                ifstream map(mapMeta);
                Vector2 initial;
                map >> initial.x >> initial.y;
                _tile_map[{initial.y, initial.y}] = vector<int>(576, -1);

                for (int i = 0; i < _map_size.x; i++) for (int j = 0; j < _map_size.y; j++) map >> _tile_map[{initial.y, initial.y}][j * _map_size.x + i];
            }
        }
    public:
        TileMap() { 
            _scale = 1.0f; 
            _set = nullptr;
            _filename = "";
        }
        TileMap(string line) {
            vector<string> aux = splice(line, ',');

            construct(aux[0], stof(aux[1]), aux[2]);
        }
        TileMap(string set, float scale, string mapMeta) {
            construct(set, scale, mapMeta);
        }

        void setScale(const float &s) {_scale = s;}
        void setTileSet(const string& path) {
            construct(path, 1.0f, "");
        }
        void addPoint(Vector2 pos, int value) {
            Vector2 aux1, aux2;
            aux1.x = pos.x < 0 ? int((-1 * pos.x) / _map_size.x) * -1 - 1 : int(pos.x / _map_size.x);
            aux1.y = pos.y < 0 ? int((-1 * pos.y) / _map_size.y) * -1 - 1 : int(pos.y / _map_size.y);

            aux2.x = pos.x < 0 ? 24 + (int)pos.x % 24 : (int)pos.x % 24;
            aux2.y = pos.y < 0 ? 24 + (int)pos.y % 24 : (int)pos.y % 24;

            if (_tile_map.find({ aux1.x, aux1.y }) == _tile_map.end()) _tile_map[{aux1.x, aux1.y}] = vector<int>(576, -1);
            _tile_map[{aux1.x, aux1.y}][aux2.x * _map_size.x + aux2.y] = value;
        }

        void draw();
        
        string filename() const {return _filename;}
        float scale() const {return _scale;}
        shared_ptr<TileSetData> tileSet() const { return _set; }

        string dataToString() {
            string data = "tilemap: " + _set->_path.substr(0, _set->_path.find('.')) + ".tsd" + ',' + ftostr(_scale, 5) + ',' + _filename;
            return data;
        }
        shared_ptr<Behaviour> clone() const { return make_shared<TileMap>(*this); }
};

class BaseUI : public Behaviour {
public:
    BaseUI() {}
    void update();
};

class Animator : public Behaviour {
private:
    EngineAnimationGraph _graph;
    string _actual, _filename;
public:
    Animator() {
    }
    Animator(string line) {
        vector<string> aux = splice(line, ',');
        _filename = aux[0];
        _graph = EngineAnimationGraph(_filename);
        _actual = aux[1];
    }

    string filename() const { return _filename; }
    string actual() const { return _actual; }

    void setActual(string a) { _actual = a; }
    void setGraph(string filename);

    void setValue(string var, int val) {
        if(_graph._variables.find(var) != _graph._variables.end()) _graph._variables[var] = val;
    }

    void update();

    string dataToString() {
        string data = "animator: " + _filename + "," + _actual;
        return data;
    }
    std::shared_ptr<Behaviour> clone() const {
        return std::make_shared<Animator>(*this);
    }
};

class PlayerController;

class GameObject : public Object, public enable_shared_from_this<GameObject>{
    private:
        string _name;
        vector<shared_ptr<Behaviour>> _behavior_list;
        shared_ptr<Nary<GameObject>> _root;
    public:
        GameObject() : Object() {}
        GameObject(const GameObject& go) : Object(go._offset, go._size), _name(go._name){}
        GameObject(Vector2 offset, Vector2 size, string name) : Object(offset, size), _name(name) {}

        template<typename N> void addBehaviour(shared_ptr<N> behavior) {
            if (this->getBehaviour<N>() == nullptr) {
                _behavior_list.push_back(behavior);
                behavior->setGameObject(shared_from_this());
            }
        }
        void removeBehaviour(const int& i) {
            if (_behavior_list.size() > i) _behavior_list.erase(_behavior_list.begin() + i);
        }
        template<typename N> shared_ptr<N> getBehaviour() const{
            for(auto i : _behavior_list) if(auto j = dynamic_pointer_cast<N>(i)) return j;
            return nullptr;
        }
        
        vector<shared_ptr<Behaviour>> getBehaviours() const;
        void moveBehaviourUp(const int& i) {
            if (_behavior_list.size() > i && i > 0) swap(_behavior_list[i-1], _behavior_list[i]);
        }
        void moveBehaviourDown(const int& i) {
            if (_behavior_list.size() - 1 > i && i >= 0) swap(_behavior_list[i + 1], _behavior_list[i]);
        }

        void setTree(shared_ptr<Nary<GameObject>> root);
        void setXOffset(const float &x);
        void setYOffset(const float &y);
        void setWidth(const float &x);
        void setHeight(const float &y);
        void setName(const string &n);

        void update(const Vector2 &offset);
        void update();
        void draw();

        Vector2 getPosition() const;
        Vector2 getOffset() const;
        Vector2 getSize() const;
        string name() const;
        ObjectID id() const;
        shared_ptr<Nary<GameObject>> root() const;
};

class Scene {
    private:
        shared_ptr<Nary<GameObject>> _root;
        string _filename;

        Nary<GameObject> getNextGameObject(ifstream &file, bool &end);
        void updatePostorder(const Nary<GameObject>& a, Vector2 offset);
        void drawPostorder(const Nary<GameObject>& a);
        void savePostorder(const Nary<GameObject>& a, ofstream& file);
    public:
        Scene();
        Scene(const string &filename);
        Scene(shared_ptr<Nary<GameObject>> root);

        void update(const Vector2 &offset = { 0.0f, 0.0f });
        void draw();

        void save();

        shared_ptr<Nary<GameObject>> getRoot() const;
};

Nary<pair<bool, shared_ptr<GameObject>>> convertToNewTree(Nary<GameObject> a);

#endif
