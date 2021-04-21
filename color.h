//
// Created by Furkan C. on 17.04.2021.
//

#ifndef COLOR_H
#define COLOR_H

#include "vec3.h"

#include <iostream>

void write_color(std::ostream &out, color pixel_color) {
    out << static_cast<int>(pixel_color.x()) << ' '
        << static_cast<int>(pixel_color.y()) << ' '
        << static_cast<int>(pixel_color.z()) << '\n';
}

void clamp255(color& col) {
    col.e[0] = (col.e[0] > 255) ? 255 : (col.e[0] < 0) ? 0 : col.e[0];
    col.e[1] = (col.e[1] > 255) ? 255 : (col.e[1] < 0) ? 0 : col.e[1];
    col.e[2] = (col.e[2] > 255) ? 255 : (col.e[2] < 0) ? 0 : col.e[2];
}

#endif

