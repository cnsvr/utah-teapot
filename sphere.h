//
// Created by Furkan C. on 18.04.2021.
//

#ifndef SPHERE_H
#define SPHERE_H

#include "vec3.h"
#include "color.h"
#include "object.h"
#include "ray.h"
#include "types.h"

bool solveQuadratic(const double &a, const double &b, const double &c, double &t0, double &t1) {

    double discrimination = b * b - 4 * a * c;
    if (discrimination < 0) return false;
    else if (discrimination == 0) {
        t0 = t1 = - 0.5 * b / a;
    }
    else {
        double q = (b > 0) ?
                   -0.5 * (b + sqrt(discrimination)) :
                   -0.5 * (b - sqrt(discrimination));
        t0 = q / a;
        t1 = c / q;
    }

    return true;
}

class sphere : public object {
private:
    objectType type;
    vec3 center;
    double radius;
    color ambientCol;
    color diffuseCol;
public:
    sphere() {}
    sphere(objectType t, vec3 cen, double r, color a, color d) : type(t), center(cen), radius(r), ambientCol(a), diffuseCol(d) {}
    vec3 getSphereCenter() const { return center; }
    double getSphereRadius() const { return radius; }
    virtual objectType getType() { return type; }
    virtual color getAmbientColor() { return ambientCol; }
    virtual color getDiffuseColor() { return diffuseCol; }
    virtual vec3 getNormalAt(vec3 pi) { return unit_vector(pi - center); }
    virtual double findIntersection(ray r) {
        double t0,t1;

        vec3 oc = r.getRayOrigin() - center;
        double a = dot(r.getRayDirection(), r.getRayDirection());
        double b = 2.0 * dot(oc, r.getRayDirection());
        double c = dot(oc, oc) - radius * radius;
        if (!solveQuadratic(a,b,c,t0,t1)) return false;

        if (t0 > t1) std::swap(t0,t1);

        if (t0 < 0) {
            t0 = t1; // if t0 is negative, let's use t1 instead
            if (t0 < 0) return -1; // both t0 and t1 are negative
        }

        return t0;
    }
};
#endif //SPHERE_H
