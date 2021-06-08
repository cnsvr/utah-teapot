//
// Created by Furkan C. on 12.05.2021.
//

#ifndef PLANE_H
#define PLANE_H

#include "vec3.h"
#include "ray.h"
#include "object.h"
#include "types.h"

class plane : public object  {
private:
    objectType type;
    vec3 normal;
    double distance;
    color col;
public:
    plane() {}
    plane(objectType t, vec3 n, double d, color c) : type(t), normal(n),distance(d), col(c) {}
    vec3 getPlaneNormal() const { return normal; }
    double getPlaneDistance() const { return distance; }

    virtual objectType getType() { return type; }
    virtual color getAmbientColor() { return col; }
    virtual color getDiffuseColor() { return col; }
    virtual vec3 getNormalAt(vec3 p) { return normal; }
    virtual double findIntersection(ray r) {
        double a = dot(r.getRayDirection(), normal);
        if (a == 0) {   // Ray is parallel to plane.
            return -1;
        } else {
          double b = dot(normal,(r.getRayOrigin() + (normal * distance)));
          return -1*b/a;
        }
    }
};

#endif