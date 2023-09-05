#include "platformerEngineBasics.h"

/*
#include "RuntimeSourceDependency.h"
RUNTIME_COMPILER_SOURCEDEPENDENCY;
// */

string getNextWord(string &line, int &actual){
    int pos = 0, wordPos = 0;
    bool found = false;

    while(!found && pos != int(line.size())){
        found = line[pos] != ' ';
        if(!found) pos += 4;
    }

    if(found){
        found = false;
        while(!found && pos + wordPos != int(line.size())){
            found = line[(size_t)pos + wordPos] == ':';
            if(!found) wordPos ++;
        }
        actual = pos + wordPos;
    } else actual = -1;

    string ret = line.substr(pos, wordPos);
    if(pos + wordPos + 2 <= (int)line.length()) line = line.substr((size_t)pos + wordPos + 2, line.length());
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
    return source.substr(prev, (size_t)last - prev);
}

vector<string> splice(string source, char divider){
    vector<string> result;
    int pos = 0;

    while(pos <= (int)source.length()) result.push_back(slice(source, divider, pos));

    return result;
}

string str_toupper(string s) {
    transform(s.begin(), s.end(), s.begin(), [](unsigned char c) { return toupper(c); });
    return s;
}
string str_tolower(string s) {
    transform(s.begin(), s.end(), s.begin(), [](unsigned char c) { return tolower(c); });
    return s;
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

void Resources::addTileset(const string& filename) {
    _tileset_list.insert(pair<string, shared_ptr<TileSetData>>(filename, make_shared<TileSetData>(filename)));
}

void Resources::addAnimation(const string& filename) {
    _animation_list.insert(pair<string, shared_ptr<EngineAnimation>>(filename, make_shared<EngineAnimation>(filename)));
}

/*
void Resources::addAnimationGraph(const string& filename) {
    _tileset_list.insert(pair<string, shared_ptr<TileSetData>>(filename, make_shared<TileSetData>(filename)));
}
// */
shared_ptr<Texture2D> Resources::texture(const string &filename, const bool &load) {
    auto search = _texture_list.find(filename);
    if(load && search == _texture_list.end()) {
        addTexture(filename);
        search = _texture_list.find(filename);
    }
    return search == _texture_list.end()? nullptr : search->second;    
}

shared_ptr<Font> Resources::font(const string &filename, const bool &load) {
    auto search = _font_list.find(filename);
    if(load && search == _font_list.end()) {
        addFont(filename);
        search = _font_list.find(filename);
    }
    return search == _font_list.end()? nullptr : search->second;
}

shared_ptr<TileSetData> Resources::tileset(const string &filename, const bool &load) {
    auto search = _tileset_list.find(filename);
    if(load && search == _tileset_list.end()) {
        addTileset(filename);
        search = _tileset_list.find(filename);
    }
    return search == _tileset_list.end()? nullptr : search->second;
}

shared_ptr<EngineAnimation> Resources::animation(const string& filename, const bool& load) {
    auto search = _animation_list.find(filename);
    if (load && search == _animation_list.end()) {
        addAnimation(filename);
        search = _animation_list.find(filename);
    }
    return search == _animation_list.end() ? nullptr : search->second;
}
/*
shared_ptr<EngineAnimationGraph> Resources::animation_graph(const string& filename, const bool& load) {
    auto search = _animation_graph_list.find(filename);
    if (load && search == _animation_graph_list.end()) {
        addAnimationGraph(filename);
        search = _animation_graph_list.find(filename);
    }
    return search == _animation_graph_list.end() ? nullptr : search->second;
}
// */

// [OBJECT ID]
ObjectID::ObjectID(bool n = true) {
    _value = "";
    if (n) {
        for (int i = 0; i < 10; i++) {
            _value += to_string(rand() % 10);
        }
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
        float length = _text.size() * ((float)_font_size * 8.0f / 16.5f);
        this_pos = Vector2{_go->_position.x + (_go->_size.x / 2) - (length / 2), _go->_position.y};
    }else if(_text_allign == 'r'){
        float length = _text.size() * ((float)_font_size * 8.0f / 16.5f);
        this_pos = Vector2{_go->_position.x + _go->_size.x - length, _go->_position.y};
    }

    DrawTextEx(_font, _text.c_str(), Vector2{this_pos.x, this_pos.y}, (float)_font_size, 0, _text_color);
}

// [SPRITE]
void Sprite::draw() {
    if(_texture != nullptr) DrawTexturePro(*_texture, { 0.0f, 0.0f, (float)_texture->width, (float)_texture->height }, { _go->_position.x, _go->_position.y, _go->_size.x, _go->_size.y }, { 0.0f, 0.0f }, 0, _color);
    else DrawRectangle((int)_go->_position.x, (int)_go->_position.y, (int)_go->_size.x, (int)_go->_size.y, _color);
}

// [ANIMATED SPRITE]
void AnimatedSprite::draw(){
    if (_animation) _animation->draw(_go->_position, _go->_size, _fliph, _flipv);
}

// [TILEMAP]
void TileMap::draw() {
    if(_set != nullptr){
        for (auto item : _tile_map) {
            for (int i = 0; i < _map_size.x; i++) {
                for (int j = 0; j < _map_size.y; j++) {
                    int val = item.second[j * _map_size.x + i];
                    if (val != -1) {
                        Vector2 new_pos = Vector2{ _go->_position.x + _set->_tileSize.x * j * _scale, _go->_position.y + _set->_tileSize.y * i * _scale };
                        new_pos.x +=  item.first.first * _set->_tileSize.x * _scale * 24;
                        new_pos.y += item.first.second * _set->_tileSize.y * _scale * 24;

                        Vector2 aux = (*_set)[val];
                        Rectangle src = Rectangle{ aux.x * _set->_tileSize.x, aux.y * _set->_tileSize.y, _set->_tileSize.x, _set->_tileSize.y };
                        Rectangle dst = Rectangle{ new_pos.x, new_pos.y, _set->_tileSize.x * _scale, _set->_tileSize.y * _scale };

                        DrawTexturePro(*_set->_texture, src, dst, Vector2{ 0.0f, 0.0f }, 0.0f, WHITE);
                    }
                }
            }
        }
    }
}

// [BASEUI]
void BaseUI::update() { 
    _go->_position = { 0.0f, 0.0f }; 
}

// [ANIMATOR]
void Animator::setGraph(string filename) {
    _filename = filename;
    _graph = EngineAnimationGraph(_filename);
    _actual = _graph.graph.GetVertex()[0];
    _go->getBehaviour<AnimatedSprite>()->setAnimation(_graph._animations[_actual]->_path);
}

void Animator::update() {
    for (auto item : _graph.graph.GetEdges(_actual)) {
        vector<string> aux = splice(item, ' ');
        bool next = false;
        // /*
        if (aux[1] == ">") next = _graph._variables[aux[0]] > stoi(aux[2]);
        else if (aux[1] == "<") next = _graph._variables[aux[0]] < stoi(aux[2]);
        else if (aux[1] == "==") next = _graph._variables[aux[0]] == stoi(aux[2]);
        else if (aux[1] == ">=") next = _graph._variables[aux[0]] >= stoi(aux[2]);
        else if (aux[1] == "<=") next = _graph._variables[aux[0]] <= stoi(aux[2]);
        // */
        if (next) {
            _actual = _graph.graph.GetNextVertex(_actual, item);
            if(_go->getBehaviour<AnimatedSprite>() != nullptr) _go->getBehaviour<AnimatedSprite>()->setAnimation(_graph._animations[_actual]->_path);
        }
    }
    // _animations[actual]->update();
}

// [GAME OBJECT]
vector<shared_ptr<Behaviour>> GameObject::getBehaviours() const {
    return _behavior_list;
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
    _position = Vector2{ _offset.x + offset.x, _offset.y + offset.y };
    update();
}

void GameObject::update(){
    for(auto i : _behavior_list) i->update();
}

void GameObject::draw(){
    for(auto i : _behavior_list) i->draw();
}

Vector2 GameObject::getPosition() const {
    return _position;
}

Vector2 GameObject::getOffset() const {
    return _offset;
}

Vector2 GameObject::getSize() const {
    return _size;
}

string GameObject::name() const {
    return _name;
}

ObjectID GameObject::id() const {
    return _id;
}

shared_ptr<Nary<GameObject>> GameObject::root() const{
    return _root;
}

// [SCENE]
Scene::Scene() {
    _root = make_shared<Nary<GameObject>>();
    _filename = "resources/NewScene.scn";
}

Scene::Scene(const string &filename) {
    _filename = filename;
    _root = make_shared<Nary<GameObject>>();
    bool ended = false;

    ifstream file(_filename);
    
    while(!ended) {
        Nary<GameObject> aux = getNextGameObject(file, ended);
        if(!ended) _root->setBrother(aux);
    }

    file.close();
}

Scene::Scene(shared_ptr<Nary<GameObject>> root) {
    _root = root;
}

void Scene::update(const Vector2& offset) {
    for(int i = 1; i <= _root->nBrothers(); i++) updatePostorder(_root->brother(i), offset);
}

void Scene::draw() {
    for(int i = 1; i <= _root->nBrothers(); i++) drawPostorder(_root->brother(i));
}

void Scene::save() {
    ofstream file(_filename);

    for (int i = 1; i <= _root->nBrothers(); i++) savePostorder(_root->brother(i), file);

    file.close();
}

Nary<GameObject> Scene::getNextGameObject(ifstream &file, bool &end){
    Nary<GameObject> result;
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
            if(part == "behavior"){
                bool endComp = false;
                while(!endComp){
                    getline(file, line);
                    comp = getNextWord(line, pos);
                    if (comp == "baseui") object->addBehaviour(make_shared<BaseUI>());
                    else if (comp == "label") object->addBehaviour(make_shared<Label>(line));
                    else if (comp == "sprite") object->addBehaviour(make_shared<Sprite>(line));
                    else if (comp == "animatedsprite") object->addBehaviour(make_shared<AnimatedSprite>(line));
                    else if (comp == "animator") object->addBehaviour(make_shared<Animator>(line));
                    else if (comp == "tilemap") object->addBehaviour(make_shared<TileMap>(line));
                    else if(comp == "end") endComp = true;
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
        if (a.content()->getBehaviour<BaseUI>() != nullptr) offset = { 0.0f, 0.0f };
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

void Scene::savePostorder(const Nary<GameObject>& a, ofstream& file) {
    if (!a.isEmpty()) {
        file << a.content()->name() << ": " << ftostr(a.content()->_offset.x, 5) << ' ' << ftostr(a.content()->_offset.y, 5) << ',' << ftostr(a.content()->_size.x, 5) << ' ' << ftostr(a.content()->_size.y, 5) << '\n';

        if (a.content()->getBehaviours().size() > 0) {
            file << "behavior:\n";
            for (auto item : a.content()->getBehaviours()) file << item->dataToString() << '\n';
            file << "end\n";
        }
        if (a.nChilds() > 0) {
            file << "objects:\n";
            for (int i = 1; i <= a.nChilds(); i++) savePostorder(a.child(i), file);
            file << "end\n";
        }
        
        file << "end\n";
    }
}

Nary<pair<bool, shared_ptr<GameObject>>> convertToNewTree(Nary<GameObject> a) {
    Nary<pair<bool, shared_ptr<GameObject>>> newTree;

    for (int i = 1; i <= a.nBrothers(); i++) {
        Nary<pair<bool, shared_ptr<GameObject>>> newChild(convertToNewTree(a.brother(i).child(0)));
        Nary<pair<bool, shared_ptr<GameObject>>> newBrother(make_shared<pair<bool, shared_ptr<GameObject>>>(pair<bool, shared_ptr<GameObject>>(false, a.brother(i).content())));

        if (!newChild.isEmpty()) newBrother.setChild(newChild);
        newTree.setBrother(newBrother);
    }

    return newTree;
}