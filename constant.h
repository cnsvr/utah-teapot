//
// Created by Furkan C. on 20.04.2021.
//

#ifndef CONSTANT_H
#define CONSTANT_H

#include "vec3.h"
#include "color.h"
#include "ray.h"
#include "object.h"
#include "types.h"

const double kInfinity = std::numeric_limits<double>::max();

double cameraToWorld[4][4]= {{ 0.897258, 0, -0.441506, 0 },
                             { -0.288129, 0.757698, -0.585556, 0 },
                             { 0.334528, 0.652606, 0.679851, 0 },
                             { 5.439442, 11.080794, 10.381341, 1} };

double worldToObject[4][4] = {{1, 0, 0, 0 },
                              {0, 0, 1, 0 },
                              {0, -1, 0, 0 },
                              {0, 0, 0, 1 }};

double transposeWorldToObject[4][4] = {{worldToObject[0][0], worldToObject[1][0], worldToObject[2][0], worldToObject[3][0]},
                                       {worldToObject[0][1], worldToObject[1][1], worldToObject[2][1], worldToObject[3][1]},
                                       {worldToObject[0][2], worldToObject[1][2], worldToObject[2][2], worldToObject[3][2]},
                                       {worldToObject[0][3], worldToObject[1][3], worldToObject[2][3], worldToObject[3][3]}};

double teapotWorld[4][4] = {{ 1, 0, 0, 0 },
                            { 0, 0, -1, 0 },
                            { 0, 1, 0, 0 },
                            { 0, 0, 0, 1 } };

void multVecMatrix(const vec3 &src, vec3 &dst)
{
    double a, b, c, w;

    a = src.e[0] * cameraToWorld[0][0] + src.e[1] * cameraToWorld[1][0] + src.e[2] * cameraToWorld[2][0] + cameraToWorld[3][0];
    b = src.e[0] * cameraToWorld[0][1] + src.e[1] * cameraToWorld[1][1] + src.e[2] * cameraToWorld[2][1] + cameraToWorld[3][1];
    c = src.e[0] * cameraToWorld[0][2] + src.e[1] * cameraToWorld[1][2] + src.e[2] * cameraToWorld[2][2] + cameraToWorld[3][2];
    w = src.e[0] * cameraToWorld[0][3] + src.e[1] * cameraToWorld[1][3] + src.e[2] * cameraToWorld[2][3] + cameraToWorld[3][3];

    dst.e[0] = a / w;
    dst.e[1] = b / w;
    dst.e[2] = c / w;
}

void multDirMatrix(const vec3 &src, vec3 &dst)
{
    double a, b, c;

    a = src.e[0] * cameraToWorld[0][0] + src.e[1] * cameraToWorld[1][0] + src.e[2] * cameraToWorld[2][0];
    b = src.e[0] * cameraToWorld[0][1] + src.e[1] * cameraToWorld[1][1] + src.e[2] * cameraToWorld[2][1];
    c = src.e[0] * cameraToWorld[0][2] + src.e[1] * cameraToWorld[1][2] + src.e[2] * cameraToWorld[2][2];

    dst.e[0] = a;
    dst.e[1] = b;
    dst.e[2] = c;
}

void multDirMatrixTransposeWorldToObject(const vec3 &src, vec3 &dst)
{
    double a, b, c;

    a = src.e[0] * transposeWorldToObject[0][0] + src.e[1] * transposeWorldToObject[1][0] + src.e[2] * transposeWorldToObject[2][0];
    b = src.e[0] * transposeWorldToObject[0][1] + src.e[1] * transposeWorldToObject[1][1] + src.e[2] * transposeWorldToObject[2][1];
    c = src.e[0] * transposeWorldToObject[0][2] + src.e[1] * transposeWorldToObject[1][2] + src.e[2] * transposeWorldToObject[2][2];

    dst.e[0] = a;
    dst.e[1] = b;
    dst.e[2] = c;
}
void multVecMatrixTeapot(const vec3 &src, vec3 &dst)
{
    double a, b, c, w;

    a = src.e[0] * teapotWorld[0][0] + src.e[1] * teapotWorld[1][0] + src.e[2] * teapotWorld[2][0] + teapotWorld[3][0];
    b = src.e[0] * teapotWorld[0][1] + src.e[1] * teapotWorld[1][1] + src.e[2] * teapotWorld[2][1] + teapotWorld[3][1];
    c = src.e[0] * teapotWorld[0][2] + src.e[1] * teapotWorld[1][2] + src.e[2] * teapotWorld[2][2] + teapotWorld[3][2];
    w = src.e[0] * teapotWorld[0][3] + src.e[1] * teapotWorld[1][3] + src.e[2] * teapotWorld[2][3] + teapotWorld[3][3];

    dst.e[0] = a / w;
    dst.e[1] = b / w;
    dst.e[2] = c / w;
}


#endif //CONSTANT_H
