#include "platformerEngineBasics.h"

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

string ftostr(float f, int precision) {
    stringstream stream;
    stream << fixed << setprecision(precision) << f;
    return stream.str();
}

unsigned char stouc(const string &source){
    return (unsigned char)stoi(source);
}

string slice(string source, char divider, int &pos){
    int prev = pos, last = pos;
    bool found = false;

    while(!found && last <= (int)source.length()){
        found = source[last] == divider;
        if(!found) last ++;
    }

    pos = last + 1;
    // cout<<"sliced word: prev: \""<<prev<<"\", last: \""<<last<<"\"\t"<<source.substr(prev, last - prev)<<"\n";
    return source.substr(prev, last - prev);
}

vector<string> splice(string source, char divider){
    vector<string> result;
    int pos = 0;

    while(pos <= (int)source.length()) result.push_back(slice(source, divider, pos));

    return result;
}

// [TILESET]
TileSet::TileSet(shared_ptr<Texture2D> texture, string meta){
    _texture = texture;

    ifstream map(meta);
    string line;
    int i = 0;

    while(getline(map, line)){
        vector<string> aux = splice(line, ' ');
        for(int j = 0; j < (int)aux.size(); j++){
            Vector2 pos{(float)j, (float)i};
            _map.insert(pair<int, Vector2>(stoi(aux[j]), pos));
        }
        i++;
    }
}

Vector2 TileSet::operator [](const int &pos) const{
    map<int, Vector2>::const_iterator found = _map.find(pos);
    if(found != _map.end()) return found->second;
    return Vector2{-1.0f, -1.0f};
}

shared_ptr<Texture2D> TileSet::getTexture() const{
    return _texture;
}

// [RESOURCES]
shared_ptr<Resources> Resources::_instance = nullptr;
shared_ptr<Resources> Resources::getInstance() {
    if(_instance == nullptr) _instance = make_shared<Resources>();
    return _instance;
}

void Resources::addTexture(const string &filename) {
    string str = filename;
    char *cstr = new char[str.length() + 1];
    strcpy(cstr, str.c_str());
    
    _texture_list.insert(pair<string, shared_ptr<Texture2D>>(filename, make_shared<Texture2D>(LoadTexture(cstr))));

}

void Resources::addFont(const string &filename) {
    string str = filename;
    char *cstr = new char[str.length() + 1];
    strcpy(cstr, str.c_str());
    
    _font_list.insert(pair<string, shared_ptr<Font>>(filename, make_shared<Font>(LoadFont(cstr))));
}

void Resources::addTileset(const string &filename) {    
    _tileset_list.insert(pair<string, shared_ptr<TileSetData>>(filename, make_shared<TileSetData>(filename)));
}

shared_ptr<Texture2D> Resources::texture(const string &filename, const bool &load = false) {
    auto search = _texture_list.find(filename);
    if(load && search == _texture_list.end()) {
        addTexture(filename);
        search = _texture_list.find(filename);
    }
    return search == _texture_list.end()? nullptr : search->second;    
}

shared_ptr<Font> Resources::font(const string &filename, const bool &load = false) {
    auto search = _font_list.find(filename);
    if(load && search == _font_list.end()) {
        addFont(filename);
        search = _font_list.find(filename);
    }
    return search == _font_list.end()? nullptr : search->second;
}

shared_ptr<TileSetData> Resources::tileset(const string &filename, const bool &load = false) {
    auto search = _tileset_list.find(filename);
    if(load && search == _tileset_list.end()) {
        addTileset(filename);
        search = _tileset_list.find(filename);
    }
    return search == _tileset_list.end()? nullptr : search->second;
}

// [OBJECT ID]
ObjectID::ObjectID() {
    _value = "";
    for(int i = 0; i < 10; i++){
        _value += to_string(rand() % 10);
    }
}

bool ObjectID::operator ==(const ObjectID & id) { 
    return _value == id._value; 
}

bool ObjectID::operator >(const ObjectID & id) { 
    return _value > id._value; 
}

void ObjectID::show() const { 
    cout << "[" << _value << "]"; 
}

// [OBJECT]
Object::Object() {
    _position = _offset = _size = Vector2{0.0f, 0.0f};
}

Object::Object(Vector2 offset, Vector2 size){
    _id = ObjectID();
    _position = _offset = offset;
    _size = size;
}

void Object::update(const Vector2 &offset) {
    _position = Vector2{_offset.x + offset.x, _offset.y + offset.y};
}

// [BEHAVIOUR]
void Behaviour::setGameObject(shared_ptr<GameObject> go) {
    _go = go;
}

// [LABEL]
void Label::draw(){
    Vector2 this_pos = _go->_position;
    
    if(_text_allign == 'c'){
        float length = _text.size() * (_font_size * 8 / 16.5);
        this_pos = Vector2{_go->_position.x + (_go->_size.x / 2) - (length / 2), _go->_position.y};
    }else if(_text_allign == 'r'){
        float length = _text.size() * (_font_size * 8 / 16.5);
        this_pos = Vector2{_go->_position.x + _go->_size.x - length, _go->_position.y};
    }
    
    DrawTextEx(_font, _text.c_str(), Vector2{this_pos.x, this_pos.y}, _font_size, 0, _text_color);
    
    // DrawTextRecEx(_font, _text->c_str(), Rectangle{position.x, position.y, size.x, size.y}, _font_size, 0, true, WHITE, 0, 0, WHITE, BLUE);
}

// [SPRITE]
void Sprite::draw() {
    Vector2 position = _go->getPosition();
    Vector2 size = _go->getSize();

    if(_texture != nullptr) DrawTextureEx(*_texture, position, 0.0f, 1, _color);
    else DrawRectangle(position.x, position.y, size.x, size.y, _color);
}

// [ANIMATED SPRITE]
AnimatedSprite::AnimatedSprite() {
    _texture = nullptr;
}

AnimatedSprite::AnimatedSprite(string filename, shared_ptr<Texture2D> texture, float timeforframe, string mapMeta) : _filename(filename), _texture(texture), _time_for_frame(timeforframe) {
    string line;
    ifstream file(mapMeta);
    int anim = 0;
    getline(file, line);
    _size = Vector2{stof(line.substr(0, line.find(','))), stof(line.substr(line.find(',') + 1, line.size()))};

    getline(file, line);
    while(line != "-1"){
        bool found = false;
        int pos = 0;
        string value = "";
        vector<int> aux;

        while(!found && pos < int(line.size())){
            found = line[pos] == ',';
            if(line[pos] == ' '){
                aux.push_back(stoi(value));
                value = "";
            } else value += line[pos];
            if(found) aux.push_back(stoi(value));
            pos ++;
        }
        _animation_map[anim] = pair<vector<int>, bool>(aux, line.substr(pos) == " true");
        anim ++;
        getline(file, line);
    }

    _tileSize = Vector2{_texture->width / _size.x, _texture->height / _size.y};
    _frame_time = _time_for_frame;
    _actual = _animation = 0;
    _fliph = _flipv = false;
}

void AnimatedSprite::update() {
    if(_texture != nullptr){
        map<int, pair<vector<int>, bool>>::iterator it = _animation_map.find(_animation);
        if(it != _animation_map.end()) _cicle = it->second.first;
        else _cicle = _cicle;
        bool loop = it->second.second;

        _frame_time -= GetFrameTime();
        if(_frame_time <= 0){
            _frame_time = _time_for_frame;
            _actual ++;
        }
        _ended = _actual > (int)_cicle.size() - 1;

        if(loop && _ended) _actual = 0;
        else if(!loop && _ended) _actual = (int)_cicle.size() - 1; 
    }
}

void AnimatedSprite::draw(){
    if(_texture != nullptr) {
        Vector2 pos = _go->_position;
        int i = _cicle[_actual] % (int)(_size.x), j = floor(_cicle[_actual] / (int)(_size.x));
        Rectangle src = Rectangle{
            i * _tileSize.x, 
            j * _tileSize.y, 
            (_fliph? -_tileSize.x : _tileSize.x), 
            (_flipv? -_tileSize.y : _tileSize.y)};
        Rectangle dst = Rectangle{pos.x, pos.y, _tileSize.x, _tileSize.y};
        DrawTexturePro(*_texture, src, dst, Vector2{0.0f, 0.0f}, 0.0f, WHITE);
    }
}

// [TILEMAP]
TileMap::TileMap() {
    
}

TileMap::TileMap(TileSet set, Vector2 size, float scale, string mapMeta) : _scale(scale) {
    _set = set; 
    _tile_set_size = size;
    _filename = mapMeta;

    ifstream map(mapMeta);
    int x, y;
    map >> x >> y;

    for(int i = 0; i < x; i++) for(int j = 0; j < y; j++) map >> _tile_map[pair<int, int>(j, i)];

    _tile_size = Vector2{_set.getTexture()->width / _tile_set_size.x, _set.getTexture()->height / _tile_set_size.y};
}

void TileMap::draw() {
    for(auto i : _tile_map){
        if(i.second != -1){
            Vector2 new_pos = Vector2{_go->_position.x + _tile_size.x * i.first.first * _scale, _go->_position.y + _tile_size.y * i.first.second * _scale};
            Vector2 aux = _set[i.second];
            Rectangle src = Rectangle{aux.x * _tile_size.x, aux.y * _tile_size.y, _tile_size.x, _tile_size.y};
            Rectangle dst = Rectangle{new_pos.x, new_pos.y, _tile_size.x * _scale, _tile_size.y * _scale};

            DrawTexturePro(*_set.getTexture(), src, dst, Vector2{0.0f, 0.0f}, 0.0f, WHITE);
            // DrawText(to_string(i.second).c_str(), dst.x + dst.width/2, dst.y, 48, WHITE);
        }
    }
}

// [GAME OBJECT]
vector<shared_ptr<Behaviour>> GameObject::getBehaviours() const {
    return _behaviour_list;
}

void GameObject::setTree(shared_ptr<Nary<GameObject>> root) {
    _root = root;
}

void GameObject::setXOffset(const float &x){
    _offset.x = x;
}

void GameObject::setYOffset(const float &y){
    _offset.y = y;
}

void GameObject::setWidth(const float &x){
    _size.x = x;
}

void GameObject::setHeight(const float &y){
    _size.y = y;
}

void GameObject::setName(const string &n){
    _name = n;
}

void GameObject::update(const Vector2 &offset){
    Object::update(offset);
    update();
}

void GameObject::update(){
    for(auto i : _behaviour_list) i->update();
}

void GameObject::draw(){
    for(auto i : _behaviour_list) i->draw();
}

Vector2 GameObject::getPosition() const {
    return _position;
}

Vector2 GameObject::getSize() const {
    return _size;
}

string GameObject::name() const {
    return _name;
}

shared_ptr<Nary<GameObject>> GameObject::root() const{
    return _root;
}

// [SCENE]
Scene::Scene() {
    _root = nullptr;
}

Scene::Scene(const string &filename) {
    _root = make_shared<Nary<GameObject>>();
    bool ended = false;

    ifstream file(filename);
    
    while(!ended) {
        Nary<GameObject> aux = getNextGameObject(file, ended);
        if(!ended) _root->setBrother(aux);
    }

    file.close();
}

Scene::Scene(shared_ptr<Nary<GameObject>> root) {
    _root = root;
}

void Scene::update(const Vector2 &offset) {
    for(int i = 1; i <= _root->nBrothers(); i++) updatePostorder(_root->brother(i), offset);
}

void Scene::update() {
    for(int i = 1; i <= _root->nBrothers(); i++) updatePostorder(_root->brother(i), Vector2{0.0f, 0.0f});
}

void Scene::draw() {
    for(int i = 1; i <= _root->nBrothers(); i++) drawPostorder(_root->brother(i));
}

Nary<GameObject> Scene::getNextGameObject(ifstream &file, bool &end){
    Nary<GameObject> result, child;
    string line, objectName, part, comp;
    int pos;
    bool ended = false;

    getline(file, line);
    objectName = getNextWord(line, pos);

    end = objectName == "end" || file.eof();
    
    if(!end){
        vector<string> aux1_1 = splice(line, ',');
        vector<string> aux2_1 = splice(aux1_1[0], ' ');
        vector<string> aux2_2 = splice(aux1_1[1], ' ');

        Vector2 position{stof(aux2_1[0]), stof(aux2_1[1])};
        Vector2 size{stof(aux2_2[0]), stof(aux2_2[1])};

        shared_ptr<GameObject> object = make_shared<GameObject>(position, size, objectName);
        object->setTree(_root);
        result = Nary<GameObject>(object);

        getline(file, line);
        part = getNextWord(line, pos);

        while(!ended){
            if(part == "behaviour"){
                bool endComp = false;
                while(!endComp){
                    getline(file, line);
                    comp = getNextWord(line, pos);
                    if(comp == "label"){
                        vector<string> aux = splice(line, ',');

                        string text = aux[0];
                        int fontSize = stoi(aux[1]);
                        char allign = aux[2].c_str()[0];

                        vector<string> aux1_2 = splice(aux[3], ' ');
                        Color color{stouc(aux1_2[0]), stouc(aux1_2[1]), stouc(aux1_2[2]), stouc(aux1_2[3])};
                        
                        object->addBehaviour(make_shared<Label>(text, fontSize, allign, color));
                    } else if (comp == "sprite") {
                        vector<string> aux = splice(line, ',');
                        vector<string> aux1_2 = splice(aux[0], ' ');
                        shared_ptr<Texture2D> tex;

                        if((int)aux.size() == 2) tex = Resources::getInstance()->texture(aux[1], true);
                        Color color{stouc(aux1_2[0]), stouc(aux1_2[1]), stouc(aux1_2[2]), stouc(aux1_2[3])};

                        if((int)aux.size() == 2) object->addBehaviour(make_shared<Sprite>(aux[1], tex, color));
                        else object->addBehaviour(make_shared<Sprite>(color));
                    } else if (comp == "animatedsprite") {
                        vector<string> aux1_1 = splice(line, ',');

                        float timeFrame = stof(aux1_1[1]);
                        string map_meta = aux1_1[2];

                        object->addBehaviour(make_shared<AnimatedSprite>(aux1_1[0], Resources::getInstance()->texture(aux1_1[0], true), timeFrame, map_meta));
                    } else if (comp == "tilemap") {
                        vector<string> aux1_1 = splice(line, ',');
                        vector<string> aux2_2 = splice(aux1_1[2], ' ');
                        
                        shared_ptr<Texture2D> tex = Resources::getInstance()->texture(aux1_1[0], true);
                        
                        Vector2 tileMapSize{stof(aux2_2[0]), stof(aux2_2[1])};
                        float tileMapScale = stof(aux1_1[3]);
                        string tileMapMeta = aux1_1[4];

                        object->addBehaviour(make_shared<TileMap>(TileSet(tex, aux1_1[1]), tileMapSize, tileMapScale, tileMapMeta));
                    } else if(comp == "end") endComp = true;
                    /* if(comp == "transform"){
                        vector<string> aux1_1 = splice(line, ',');
                        vector<string> aux2_1 = splice(aux1_1[0], ' ');
                        vector<string> aux2_2 = splice(aux1_1[1], ' ');

                        Vector2 position{stof(aux2_1[0]), stof(aux2_1[1])};
                        Vector2 scale{stof(aux2_2[0]), stof(aux2_2[1])};
                        
                        object->addBehaviour(make_shared<TransformComp>(position, scale));
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

                        object->addBehaviour(make_shared<TileMap>(TileMap(t, tileMapSize, tileMapScale, tileMapMeta)));
                    }else if(comp == "paint"){
                        vector<string> aux = splice(line, ' ');
                        Color color{stouc(aux[0]), stouc(aux[1]), stouc(aux[2]), stouc(aux[3])};

                        object->addBehaviour(make_shared<Paint>(Paint(color)));
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

                        object->addBehaviour(make_shared<AnimatedSprite>(AnimatedSprite(aux1_1[0], tex, scale, map_meta, timeFrame)));
                    }else if(comp == "aabb"){
                        vector<string> aux1_1 = splice(line, ',');
                        
                        object->addBehaviour(make_shared<AABB>(AABB(aux1_1[0] == "true", aux1_1[1] == "true")));
                    }else if(comp == "button"){
                        vector<string> aux1 = splice(line, ',');

                        vector<string> aux2 = splice(aux1[0], ' ');
                        Color color{stouc(aux2[0]), stouc(aux2[1]), stouc(aux2[2]), stouc(aux2[3])};
                        if(aux1.size() > 1){
                            vector<string> aux3 = splice(aux1[1], ' ');
                            Color hover{stouc(aux3[0]), stouc(aux3[1]), stouc(aux3[2]), stouc(aux3[3])};
                            object->addBehaviour(make_shared<Button>(Button(color, hover)));
                        } else object->addBehaviour(make_shared<Button>(Button(color)));
                    }else if(comp == "label"){
                        vector<string> aux = splice(line, ',');

                        // shared_ptr<Font> font;

                        // if(_fontlist.find(aux[1]) != _fontlist.end()){
                        //     font = _fontlist.find(aux[1])->second;
                        // }else{
                        //     string str = "resources/" + aux[1];
                        //     char *cstr = new char[str.length() + 1];
                        //     strcpy(cstr, str.c_str());
                        //     font = make_shared<Font>(LoadFont(cstr));
                        //     _fontlist.insert(pair<string, shared_ptr<Font>>(aux[1], font));
                        // }

                        string text = aux[0];
                        // char allign = aux[2].c_str()[0];
                        int fontSize = stoi(aux[1]);
                        // vector<string> aux1_2 = splice(aux[4], ' ');
                        // Color color{stouc(aux1_2[0]), stouc(aux1_2[1]), stouc(aux1_2[2]), stouc(aux1_2[3])};

                        // object->addBehaviour(make_shared<Label>(text, font, allign, fontSize, color));
                        object->addBehaviour(make_shared<Label>(text, fontSize));
                    }else if(comp == "camera"){
                        object->addBehaviour(make_shared<TrackCamera>(TrackCamera()));
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

                        object->addBehaviour(make_shared<Mosaic>(Mosaic(tex, stof(aux1[1]))));
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

                        object->addBehaviour(make_shared<Sprite>(Sprite(aux1[0], tex, stof(aux1[1]))));
                    }else if(comp == "animation"){
                        object->addBehaviour(make_shared<Animation>(Animation(line)));
                    }else if(comp == "rigidbody"){
                        object->addBehaviour(make_shared<RigidBody>(RigidBody()));
                    }else if(comp == "selector"){
                        object->addBehaviour(make_shared<Selector>(Selector()));
                    else serializeMonoBehavior(comp, object);*/
                }
            }else if(part == "objects") {
                bool endObj = false;
                while(!endObj) {
                    Nary<GameObject> aux = getNextGameObject(file, endObj);
                    if(!endObj) result.setChild(aux);
                }
            } 
            if(part == "end") ended = true;
            else {
                getline(file, line);
                part = getNextWord(line, pos);
                ended = part == "end";
            }
        }
    }

    return result;
}

shared_ptr<Nary<GameObject>> Scene::getRoot() const {
    return _root;
}

void Scene::updatePostorder(const Nary<GameObject> &a, Vector2 offset){
    if(!a.isEmpty()) {
        a.content()->update(offset);
        offset = a.content()->_position;
        for(int i = 1; i <= a.nChilds(); i++) updatePostorder(a.child(i), offset);
    }
}

void Scene::drawPostorder(const Nary<GameObject> &a){
    if(!a.isEmpty()) {
        a.content()->draw();
        for(int i = 1; i <= a.nChilds(); i++) drawPostorder(a.child(i));
    }
}