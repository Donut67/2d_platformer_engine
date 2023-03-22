#include "../include/raylib.h"
#include "../include/playerController.h"
#include <cstdlib>
#include <iostream>
#include <string>
#include <cmath>

using namespace std;

Vector2 limit(Vector2 vec, float mag){
    Vector2 aux_vec = vec;
    float Mag = sqrt(aux_vec.x * aux_vec.x + aux_vec.y * aux_vec.y);
    aux_vec = Mag < mag ? vec : Vector2{aux_vec.x / Mag * mag, aux_vec.y / Mag * mag};
    return aux_vec;
}

PlayerController::PlayerController(){
    _jumping = _falling = _wallSlide = _animation = false;
    _jumpOnWallSlide = true;
    _jumpOnWallSlideCoolDown = 0;

    _startJump     = 3500;
    _movementSpeed = 1600;
}

void PlayerController::update(){
    _speed.x = 0;

    _top = _back = _bottom = _front = false;

    NTree<GameObject> go = _gameObject->getRoot()->find(_gameObject->name());

    _top    = (*go.find("topPlayerHitbox").content())[AABB()]->getActive();
    _back   = (*go.find("rightPlayerHitbox").content())[AABB()]->getActive();
    _bottom = (*go.find("bottomPlayerHitbox").content())[AABB()]->getActive();
    _front  = (*go.find("leftPlayerHitbox").content())[AABB()]->getActive();
    // _top    = true;
    // _back   = true;
    // _bottom = true;
    // _front  = true;

    if(!_animation){
        if(IsKeyDown(KEY_A)){
            if(!_wallSlide) _speed.x -= _movementSpeed;
            (*_gameObject)[AnimatedSprite()]->setFlipH(true);
        }
        if(IsKeyDown(KEY_D)){
            if(!_wallSlide) _speed.x += _movementSpeed;
            (*_gameObject)[AnimatedSprite()]->setFlipH(false);
        }

        if(IsKeyPressed(KEY_SPACE)){
            (*_gameObject)[RigidBody()]->addForce(Vector2{0.0f, 800.0f});
        }

        // if(!_bottom){
        //     if((_jumping || _falling) && !_wallSlide) _speed.y += (_startJump / 30);
        //     else if(_wallSlide || (_front && IsKeyDown(KEY_D)) || (_back && IsKeyDown(KEY_A))) _speed.y = 60;
        // }

        // if(_jumpOnWallSlideCoolDown <= 0 && !_jumpOnWallSlide) {
        //     _jumpOnWallSlide = true;
        //     _jumpOnWallSlideCoolDown = 0;
        // }else if(_jumpOnWallSlideCoolDown > 0 && !_jumpOnWallSlide){
        //     _jumpOnWallSlideCoolDown -= GetFrameTime();
        // }
        
        // if(_wallSlide && ((!_front && !_back) || (_bottom))) {
        //     _wallSlide = false;
        // }else if((_jumping || _falling) && (_back || _front) && (IsKeyDown(KEY_D) || IsKeyDown(KEY_A)) && !_wallSlide && !_jumpOnWallSlideCoolDown != 0) {
        //     _speed.y = 10;
        //     _jumping = false;
        //     _wallSlide = _falling = true;
        // }else if(!_jumping && (_bottom || (_jumpOnWallSlide && _wallSlide)) && IsKeyPressed(KEY_SPACE)){
        //     if(_jumpOnWallSlide && _wallSlide && !_bottom){
        //         _jumpOnWallSlide = _wallSlide = false;
        //         _jumpOnWallSlideCoolDown = 0.65f;
        //     }
        //     _speed.y = -_startJump;
        //     _jumping = true;
        // }else if(_jumping && _speed.y == 0) {
        //     _jumping = false;
        //     _falling = true;
        // }else if((_falling || _jumping) && _bottom){
        //     _falling = _jumping = _wallSlide = false;
        //     _speed.y = 0;
        // }else if(!_jumping && !_bottom) {
        //     _falling = true;
        // }else if(_top) {
        //     _falling = true;
        //     _jumping = false;
        //     _speed.y = 0;
        // }
    }

    if(_wallSlide) (*_gameObject)[AnimatedSprite()]->setAnimation(1);
    else (*_gameObject)[AnimatedSprite()]->setAnimation(0);

    // _speed = limit(_speed, _startJump);
    float y = (*_gameObject)[RigidBody()]->getVelocity().y;
    (*_gameObject)[RigidBody()]->setVelocity(Vector2{_speed.x, y});

    // (*_gameObject)[TransformComp()]->setPosition(Vector2{pos.x + _speed.x * GetFrameTime(), pos.y + _speed.y * GetFrameTime()});
}

void PlayerController::setGameObject(shared_ptr<GameObject> go){
    _gameObject = go;
}
float PlayerController::getStartJump() const{
    return _startJump;
}
Vector2 PlayerController::getSpeed() const{
    return _speed;
}

void PlayerController::draw(){

}