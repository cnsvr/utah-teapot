//
// Created by Furkan C on 9.06.2021.
//

#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "types.h"
#include "vec3.h"
#include "color.h"
#include "object.h"
#include "ray.h"
#include <cmath>

static const float kEpsilon = 1e-8;

class triangle : public object {
private:
    objectType type;
    int index;
    vec3 v0;
    vec3 v1;
    vec3 v2;
    color ambientCol;
    color diffuseCol;
public:
    triangle() {}
    triangle(objectType t, int i, vec3 v0, vec3 v1, vec3 v2, color a, color d) :  type(t), index(i), v0(v0), v1(v1), v2(v2), ambientCol(a), diffuseCol(d) { }
    virtual objectType getType() { return type; }
    int getIndex() { return index; }
    virtual color getAmbientColor() { return ambientCol; }
    virtual color getDiffuseColor() { return diffuseCol; }
    virtual vec3 getNormal() { return cross((v1-v0), (v2-v0)); }
    virtual double findIntersection(ray r, double &u, double &v) {
        double t;
        vec3 v0v1 = v1 - v0;
        vec3 v0v2 = v2 - v0;
        vec3 pvec = cross(r.getRayDirection(),v0v2);
        float det = dot(v0v1,pvec);

        // ray and triangle are parallel if det is close to 0
        if (fabs(det) < kEpsilon) return -1;

        float invDet = 1 / det;

        vec3 tvec = (r.getRayOrigin()) - v0;

        u = dot(tvec, pvec) * invDet;
        if (u < 0 || u > 1) return -1;


        vec3 qvec = cross(tvec,v0v1);
        v = dot(r.getRayDirection(),qvec) * invDet;
        if (v < 0 || u + v > 1) return -1;

        t = dot(v0v2,qvec) * invDet;

        // cout << t << endl;

        return (t > 0.0) ? t : -1;
    }
};

#endif //TRIANGLE_H
