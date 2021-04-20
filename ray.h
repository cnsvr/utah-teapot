//
// Created by Furkan C. on 17.04.2021.
//

#ifndef RAY_H
#define RAY_H

#include "vec3.h"
#include "sphere.h"

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

class ray {
public:
    ray() {}
    ray(const point3& origin, const vec3& direction)
            : orig(origin), dir(direction) {}

    point3 origin() const  { return orig; }
    vec3 direction() const { return dir; }
    point3 at(double t) const {
        return orig + t * dir;
    }
    bool intersect(const sphere& s, double &t) {
        double t0,t1;

        vec3 oc = orig - s.center;
        double a = dot(dir, dir);
        double b = 2.0 * dot(oc, dir);
        double c = dot(oc, oc) - s.radius * s.radius;
        if (!solveQuadratic(a,b,c,t0,t1)) return false;

        if (t0 > t1) std::swap(t0,t1);

        if (t0 < 0) {
            t0 = t1; // if t0 is negative, let's use t1 instead
            if (t0 < 0) return false; // both t0 and t1 are negative
        }

        t = t0;

        return true;
    }

public:
    point3 orig;
    vec3 dir;
};

#endif
