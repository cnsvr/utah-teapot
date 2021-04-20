//
// Created by Furkan C. on 18.04.2021.
//

#ifndef SPHERE_H
#define SPHERE_H

#include "vec3.h"

class sphere {
public:
    point3 center;
    double radius;
    vec3 color;
public:
    sphere() {}
    sphere(point3 c, double r, vec3 col) : center(c), radius(r), color(col) {}
    point3 c() const { return center; }
    double r() const { return radius; }
    vec3 col() const { return color; }
    vec3 getNormal(const vec3& pi) const { return (pi - center) / radius ; }
};
#endif //SPHERE_H
