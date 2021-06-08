//
// Created by Furkan C. on 12.05.2021.
//

#ifndef SOURCE_H
#define SOURCE_H

#include "vec3.h"
#include "color.h"

class source {
public:
    source() {}

    virtual vec3 getLightPosition () { return {0,0,0}; }
    virtual color getLightColor () { return {1,1,1}; }
};


#endif //SOURCE_H
