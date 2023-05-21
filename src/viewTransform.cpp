#include "../include/viewTransform.h"
#include "myFunctions.h"

void ViewTransform::show(){
    DrawRectangleRec(Rectangle{_pos.x + 2.0f, _pos.y + 1.0f, 446, 25.0f}, Color{80, 80, 80, 255});
    DrawTextEx(_font, "TRANSFORM", Vector2{_pos.x + 7.0f, _pos.y + 2.0f}, 24, 1, WHITE);

    DrawTextEx(_font, "POSITION:", Vector2{_pos.x + 7.0f, _pos.y + 2.0f + 25.0f}, 24, 1, WHITE);
    _x.setPosition(Vector2{_pos.x + 7.0f + 160.0f, _pos.y + 2.0f + 25.0f});
    _x.draw();
    _y.setPosition(Vector2{_pos.x + 7.0f + 280.0f, _pos.y + 2.0f + 25.0f});
    _y.draw();

    DrawTextEx(_font, "SCALE:", Vector2{_pos.x + 7.0f, _pos.y + 2.0f + 50.0f}, 24, 1, WHITE);
    _width.setPosition(Vector2{_pos.x + 7.0f + 160.0f, _pos.y + 2.0f + 50.0f});
    _width.draw();
    _height.setPosition(Vector2{_pos.x + 7.0f + 280.0f, _pos.y + 2.0f + 50.0f});
    _height.draw();

    DrawLineEx(Vector2{_pos.x, _pos.y + 77.0f}, Vector2{_pos.x + 450, _pos.y + 77.0f}, 2, BLACK);
}

void ViewTransform::update(shared_ptr<GameObject> go, const Vector2 &pos){
    _transform = go->getComponent<TransformComp>();
    _pos = pos;
    
    Vector2 position = _transform->getGlobalPosition();
    Vector2 scale = _transform->getScale();

    _x.setValue(position.x);
    _y.setValue(position.y);
    _width.setValue(scale.x);
    _height.setValue(scale.y);

    _x.update();
    _y.update();
    _width.update();
    _height.update();

    position.x = _x.getValue();
    position.y = _y.getValue();
    scale.x = _width.getValue();
    scale.y = _height.getValue();

    _transform->setPosition(position);
    _transform->setScale(scale);

    show();
}