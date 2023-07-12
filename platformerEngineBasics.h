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

class TileSetData;

class Resources {
    private:
        static shared_ptr<Resources> _instance;
        map<string, shared_ptr<Texture2D>> _texture_list;
        map<string, shared_ptr<Font>> _font_list;
        map<string, shared_ptr<TileSetData>> _tileset_list;
    protected:
    public:
        Resources() {}
        static shared_ptr<Resources> getInstance();

        void addTexture(const string &filename);
        void addFont(const string &filename);
        void addTileset(const string &filename);

        shared_ptr<Texture2D> texture(const string &filename, const bool &load);
        shared_ptr<Font> font(const string &filename, const bool &load);
        shared_ptr<TileSetData> tileset(const string &filename, const bool &load);
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
        }

        TileSetData(string path) {
            _path = path;
            string line;

            ifstream file(_path);
            getline(file, line);

            vector<string> aux = splice(line, ' ');
            _texture  = Resources::getInstance()->texture(aux[0], true);
            _nTiles   = Vector2{stof(aux[1]), stof(aux[2])};
            _tileSize = Vector2{stof(aux[3]), stof(aux[4])};
        }

        void setRows(int i) {
            _nTiles.y = (float)i;
            _tileSize.y = _texture->height / _nTiles.y;
        }
        void setColumns(int i) {
            _nTiles.x = (float)i;
            _tileSize.x = _texture->width / _nTiles.x;
        }

};

typedef struct EngineAnimation{
    string _name;
    vector<pair<int, float>> _sequence;
    bool _loop;
    int _actual;
    float _frame_time;
    shared_ptr<TileSetData> _data;

    EngineAnimation() {
        _frame_time = 0.0f;
        _data = nullptr;
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
        _frame_time -= GetFrameTime();

        if(_frame_time <= 0){
            _frame_time = _sequence[_actual].second;
            _actual ++;
        }

        bool ended = _actual > (int)_sequence.size() - 1;

        if(_loop && ended) _actual = 0;
        else if(!_loop && ended) _actual = (int)_sequence.size() - 1; 
    }

    void setTime(int pos, float time) {
        _sequence[pos].second = time;
    }

    void setData(shared_ptr<TileSetData> data) {
        _data = data;
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

    void debug() const {
        for(auto item : _sequence) cout << item.first << ": " << item.second << ", ";
        cout << "\n";
    }

} EngineAnimation;

struct Padding {
    float top, right, bottom, left;
    Padding() { top = right = bottom = left = 0.0f; }
    Padding(float val) { top = right = bottom = left = val; }
    Padding(float horizontal, float vertical) { top = bottom = vertical; right = left = horizontal; }
    Padding(float t, float r, float b, float l) { top = t; right = r; bottom = b; left = l; }
};

class TileSet{
    private:
        shared_ptr<Texture2D> _texture;
        map<int, Vector2> _map;
        Vector2 _tileSize;
    public:
        TileSet() {
            _tileSize = Vector2{ 0.0f, 0.0f };
        }
        TileSet(shared_ptr<Texture2D> texture, string meta);

        Vector2 operator [](const int &pos) const;
        shared_ptr<Texture2D> getTexture() const;
};

class ObjectID {
    private:
        string _value;
    public:
        ObjectID();
        
        bool operator ==(const ObjectID & id);
        bool operator >(const ObjectID & id);
        
        void show() const;
};

class Object {
    protected:
        ObjectID _id; 
    public:
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
        shared_ptr<GameObject> _go;
    public:
        virtual void update() {}
        virtual void draw() {}

        virtual void setGameObject(shared_ptr<GameObject> go);
};

class Label : public Behaviour{
    private:
        string _text;
        Font _font;
        char _text_allign;
        int _font_size;
        Color _text_color;
    public:
        Label(string text, int font_size) : _text(text), _font_size(font_size){
            _text_allign = 'l';
            _font = *Resources::getInstance()->font("resources/monogram.ttf", true);
            _text_color = WHITE;
        }
        Label(string text, int font_size, char text_allign) : _text(text), _font_size(font_size) {
            _text_allign = text_allign;
            _font = *Resources::getInstance()->font("resources/monogram.ttf", true);
            _text_color = WHITE;
        }
        Label(string text, int font_size, char text_allign, Color color) : _text(text), _font_size(font_size) {
            _text_allign = text_allign;
            _font = *Resources::getInstance()->font("resources/monogram.ttf", true);
            _text_color = color;
        }
        Label(string text, int font_size, char text_allign, Font font) : _text(text), _font_size(font_size) {
            _text_allign = text_allign;
            _text_color = WHITE;
            _font = font;
        }
        Label(string text, int font_size, char text_allign, Font font, Color color) :  _text(text), _font_size(font_size) {
            _text_allign = text_allign;
            _font = font;
            _text_color = color;
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
};

class Sprite : public Behaviour {
    private:
        string _filename;
        shared_ptr<Texture2D> _texture;
        Color _color;
    public:
        Sprite(Color color) : _color(color) {}
        Sprite(string filename, shared_ptr<Texture2D> texture, Color color) : _filename(filename), _texture(texture), _color(color) {}
        
        void setfilename(const string &name) {_filename = name;}
        void settexture(shared_ptr<Texture2D> text) {_texture = text;}
        void setcolor(const Color &c) {_color = c;}

        void draw();
        
        string filename() const {return _filename;}
        shared_ptr<Texture2D> texture() const {return _texture;}
        Color color() const {return _color;}
};

class AnimatedSprite : public Behaviour{
    private:
        string _filename;
        shared_ptr<Texture2D> _texture;
        float _time_for_frame, _frame_time;
        int _actual, _animation;
        vector<int> _cicle;
        map<int, pair<vector<int>, bool>> _animation_map;
        bool _ended, _fliph, _flipv;
        Vector2 _size, _tileSize;
    public:
        AnimatedSprite();
        AnimatedSprite(string filename, shared_ptr<Texture2D> texture, float timeforframe, string mapMeta);

        void update();
        void draw();

        string filename() const {return _filename;}
        float time_for_frame() const {return _time_for_frame;}
        Vector2 size() const {return _size;}
        bool fliph() const {return _fliph;}
        bool flipv() const {return _flipv;}
};

class TileMap : public Behaviour {
    private:
        string _filename;
        Vector2 _tile_set_size, _tile_size;
        TileSet _set;
        map<pair<int, int>, int> _tile_map;
        float _scale;
    public:
        TileMap();
        TileMap(TileSet set, Vector2 size, float scale, string mapMeta);

        void setScale(const float &s) {_scale = s;}

        void draw();
        
        string filename() const {return _filename;}
        Vector2 tile_set_size() const {return _tile_set_size;}
        float scale() const {return _scale;}
};

class GameObject : public Object, public enable_shared_from_this<GameObject>{
    private:
        string _name;
        vector<shared_ptr<Behaviour>> _behaviour_list;
        shared_ptr<Nary<GameObject>> _root;
    public:
        GameObject() : Object() {}
        GameObject(Vector2 offset, Vector2 size, string name) : Object(offset, size), _name(name) {}

        template<typename N> void addBehaviour(shared_ptr<N> behaviour){
            if(this->getBehaviour<N>() == nullptr) {
                _behaviour_list.push_back(behaviour);
                behaviour->setGameObject(shared_from_this());
            }
        }
        template<typename N> shared_ptr<N> getBehaviour() const{
            for(auto i : _behaviour_list) if(auto j = dynamic_pointer_cast<N>(i)) return j;
            return nullptr;
        }
        vector<shared_ptr<Behaviour>> getBehaviours() const;

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
        Vector2 getSize() const;
        string name() const;
        shared_ptr<Nary<GameObject>> root() const;
};

class Scene {
    private:
        shared_ptr<Nary<GameObject>> _root;

        Nary<GameObject> getNextGameObject(ifstream &file, bool &end);
        void updatePostorder(const Nary<GameObject> &a, Vector2 offset);
        void drawPostorder(const Nary<GameObject> &a);
    public:
        Scene();
        Scene(const string &filename);
        Scene(shared_ptr<Nary<GameObject>> root);

        void update(const Vector2 &offset);
        void update();
        void draw();

        shared_ptr<Nary<GameObject>> getRoot() const;
};

#endif