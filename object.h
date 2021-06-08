//
// Created by Furkan C. on 12.05.2021.
//

#ifndef OBJECT_H
#define OBJECT_H

#include "vec3.h"
#include "color.h"
#include "types.h"

using namespace std;

class object {
public:
    object() {}
    virtual objectType getType() { return PLANE ; }
    virtual color getAmbientColor () { return color(0.0,0.0,0.0); }
    virtual color getDiffuseColor () { return  color(0.0, 0.0, 0.0); }
    virtual vec3 getNormalAt(vec3 ip) { return vec3(0,0,0); }
    virtual double findIntersection(ray r) { return 0.0; }
};

#endif // OBJECT_H
