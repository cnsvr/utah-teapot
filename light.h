//
// Created by Furkan C. on 13.05.2021.
//

#ifndef LIGHT_H
#define LIGHT_H

#include "vec3.h"
#include "color.h"
#include "source.h"

class light : public source {
private:
    vec3 position;
    color col;
public:
    light () {}
    light (vec3 pos, color col) : position(pos), col(col) {}
    virtual vec3 getLightPosition () { return position; }
    virtual color getLightColor () { return col; }
};

#endif // LIGHT_H
