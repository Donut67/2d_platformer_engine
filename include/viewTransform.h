#ifndef __VIEW_TRANSFORM_H__
#define __VIEW_TRANSFORM_H__

#include "viewer.h"
#include "subject.h"
#include "raylib.h"

class ViewTransform : public Viewer, public Observer {
    private:
        Data _data;
        string _name;

    public: 
        ViewTransform(string name) : _name(name){}
        void show();
        void update(const Data& data);
};
#endif