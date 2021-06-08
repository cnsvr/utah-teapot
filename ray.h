//
// Created by Furkan C. on 17.04.2021.
//

#ifndef RAY_H
#define RAY_H

#include "vec3.h"

class ray {
public:
    ray() {}
    ray(vec3 origin, vec3 direction)
            : orig(origin), dir(direction) {}

    vec3 getRayOrigin() const  { return orig; }
    vec3 getRayDirection() const { return dir; }

    vec3 pointAt(double t) const {
        return orig + (t * dir);
    }
private:
    vec3 orig;
    vec3 dir;
};

#endif
