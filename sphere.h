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
    color col;
public:
    sphere() {}
    sphere(point3 cen, double r, color c) : center(cen), radius(r), col(c) {}
    point3 c() const { return center; }
    double r() const { return radius; }
    vec3 color() const { return col; }
    vec3 getNormal(const vec3& pi) const { return (pi - center) / radius ; }
};
#endif //SPHERE_H
