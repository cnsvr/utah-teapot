#include <vector>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <stdio.h>
#include "constant.h"
#include "triangle.h"
#include "teapot_data.h"
#include <cmath>
#include <algorithm>


using namespace std;

uint32_t divs = 8;
vec3 *P = new vec3[(divs+1)*(divs+1)];
vec3 *points = new vec3[(divs+1)*(divs+1)];
vec3 *normals = new vec3[(divs+1)*(divs+1)];
vec3 *N = new vec3[(divs+1)*(divs+1)];
int * trisIndex = new int[128 * 3];

struct IsectInfo
{
    object *hitObject = nullptr;
    float tNear = kInfinity;
    double u;
    double v;
    uint32_t index = 0;
};

color rimColor(0.2,0.4,0.3);
color bodyColor(0.6,0.1,0.7);
color handleColor(0.35,0.15,0.8);
color spoutColor(0.65,0.1,0.75);
color lidColor(0.8,0.3,0.9);



void write_color(std::ostream &out, color pixel_color) {
    out << static_cast<int>(pixel_color.getColorRed() * 255) << ' '
        << static_cast<int>(pixel_color.getColorGreen() * 255) << ' '
        << static_cast<int>(pixel_color.getColorBlue() * 255) << '\n';
}


vec3 evalBezierCurve(const vec3 *P, const float &t)
{
    float b0 = (1 - t) * (1 - t) * (1 - t);
    float b1 = 3 * t * (1 - t) * (1 - t);
    float b2 = 3 * t * t * (1 - t);
    float b3 = t * t * t;

    return P[0] * b0 + P[1] * b1 + P[2] * b2 + P[3] * b3;
}

vec3 evalBezierPatch(const vec3 *controlPoints, const float &u, const float &v)
{
    vec3 uCurve[4];
    for (int i = 0; i < 4; ++i)
        uCurve[i] = evalBezierCurve(controlPoints + 4 * i, u);

    return evalBezierCurve(uCurve, v);
}

vec3 derivBezier(const vec3 *P, const float &t)
{
    return -3 * (1 - t) * (1 - t) * P[0] +
           (3 * (1 - t) * (1 - t) - 6 * t * (1 - t)) * P[1] +
           (6 * t * (1 - t) - 3 * t * t) * P[2] +
           3 * t * t * P[3];
}

vec3 dUBezier(const vec3 *controlPoints, const float &u, const float &v)
{
    vec3 P[4];
    vec3 vCurve[4];
    for (int i = 0; i < 4; ++i) {
        P[0] = controlPoints[i];
        P[1] = controlPoints[4 + i];
        P[2] = controlPoints[8 + i];
        P[3] = controlPoints[12 + i];
        vCurve[i] = evalBezierCurve(P, v);
    }

    return derivBezier(vCurve, u);
}

vec3 dVBezier(const vec3 *controlPoints, const float &u, const float &v)
{
    vec3 uCurve[4];
    for (int i = 0; i < 4; ++i) {
        uCurve[i] = evalBezierCurve(controlPoints + 4 * i, u);
    }

    return derivBezier(uCurve, v);
}

void generateTriangles(vector<object*> &objects) {
    vec3 controlPoints[16];
    uint32_t vertices[divs * divs * 4];

    for (uint16_t j = 0, k = 0; j < divs; ++j) {
        for (uint16_t i = 0; i < divs; ++i, ++k) {
            vertices[k * 4    ] = (divs + 1) * j + i;
            vertices[k * 4 + 1] = (divs + 1) * j + i + 1;
            vertices[k * 4 + 2] = (divs + 1) * (j + 1) + i + 1;
            vertices[k * 4 + 3] = (divs + 1) * (j + 1) + i;
        }
    }

    for (int n = 0; n < 28; ++n) { // total patch number
        // set the control points for the current patch
        for (int j = 0, a = 0; j < ORDER + 1; ++j) {
            for (int k = 0; k < ORDER + 1 ; ++k, ++a) {
                controlPoints[a].e[0] =  teapot_cp_vertices[teapot_patches[n][j][k]-1].x; // x
                controlPoints[a].e[1] =  teapot_cp_vertices[teapot_patches[n][j][k]-1].y; // y
                controlPoints[a].e[2] =  teapot_cp_vertices[teapot_patches[n][j][k]-1].z; // z
            }
        }

        // generate triangles
        // there are 8 * 8 grids and each grid is composed of two triangles.
        // Total points -> (8+1) * (8+1) = 81 points in a bezier patch.
        for (int x = 0, l = 0; x <= divs ; ++x) {
            float v = x / (float)divs;
            for (int y = 0; y <= divs ; ++y, ++l) {
                float u = y / (float)divs;
                points[l] = evalBezierPatch(controlPoints, u, v);
                multVecMatrixTeapot(points[l], P[l]);
                vec3 dU = dUBezier(controlPoints, u, v);
                vec3 dV = dVBezier(controlPoints, u, v);
                normals[l] = unit_vector(cross(dU,dV));
                multDirMatrixTransposeWorldToObject(normals[l], N[l]);
            }
        }


        // Triangle index array
        for (uint32_t i = 0, k = 0, l = 0; i < divs * divs; ++i) { // for each  face
            for (uint32_t j = 0; j < 2; ++j) { // for each triangle in the face
                trisIndex[l] = vertices[k];
                trisIndex[l + 1] = vertices[k + j + 1];
                trisIndex[l + 2] = vertices[k + j + 2];
                l += 3;
            }
            k += 4;
        }

        // Create all rectangles.
        uint32_t j = 0;
        for (uint32_t i = 0; i < 128 ; ++i) {
            vec3 v0 = P[trisIndex[j]];
            vec3 v1 = P[trisIndex[j + 1]];
            vec3 v2 = P[trisIndex[j + 2]];
            triangle* tri;
            if (n < 4) {
                tri = new triangle(TRIANGLE, i, v0, v1, v2, rimColor, rimColor);
            } else if (n < 12) {
                tri = new triangle(TRIANGLE, i, v0, v1, v2, bodyColor, bodyColor);
            } else if (n < 16) {
                tri = new triangle(TRIANGLE, i, v0, v1, v2, handleColor, handleColor);
            } else if (n < 20) {
                tri = new triangle(TRIANGLE, i, v0, v1, v2, spoutColor, spoutColor);
            } else {
                tri = new triangle(TRIANGLE, i, v0, v1, v2, lidColor, lidColor);
            }
            // triangle* tri = new triangle(TRIANGLE, i, v0, v1, v2, color(0.5, 0.5, 0.5), color(0.5, 0.5, 0.5));
            objects.push_back(dynamic_cast<object*>(tri));

            j += 3;
        }
    }

}


/*
 * Calculates reflection direction.
 */
vec3 reflect(const vec3 &I, const vec3 &N)
{
    return I - 2 * dot(I, N) * N;
}


/*
 * Returns final color of pixel consist of ambient color and diffuse color.
 */

color getColorAt(vec3 intersection_point, vec3 intersection_ray_direction, vector<object*> objects, IsectInfo &isectInfo, vec3 hitNormal) {
    object *hitObject = isectInfo.hitObject;
    color ambientColor = hitObject->getAmbientColor();

    return ambientColor.scalar(std::max(0.0, dot(-hitNormal, intersection_ray_direction)));

}

int main(int argc, char** argv){

    vector<object*> objects;

    // Image

    const double aspect_ratio = 1.0;
    const int image_width = 1000;
    const int image_height = static_cast<int>(image_width / aspect_ratio);

    // Camera

    const vec3 ray_origin(cameraToWorld[3][0] / cameraToWorld[3][3],
                          cameraToWorld[3][1] / cameraToWorld[3][3],
                          cameraToWorld[3][2] / cameraToWorld[3][3]);


    generateTriangles(objects);

    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";
    auto timeStart = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < image_height; ++i) {
        for (int j = 0; j < image_width; ++j) {
            float u = (2 * (j + 0.5) / (float)image_width - 1);
            float v = (1 - 2 * (i + 0.5) / (float)image_height);

            vec3 ray_dir(u * cameraToWorld[0][0] + v * cameraToWorld[1][0] - cameraToWorld[2][0],
                         u * cameraToWorld[0][1] + v * cameraToWorld[1][1] - cameraToWorld[2][1],
                         u * cameraToWorld[0][2] + v * cameraToWorld[1][2] - cameraToWorld[2][2]);
            ray_dir = unit_vector(ray_dir);
            ray r(ray_origin, ray_dir);

            color pixel_color = color(0.1,0.1,0.1);   // Default scene color

            double tNear = kInfinity;
            IsectInfo isectInfo;
            for (int index = 0; index < objects.size(); index++) {
                double u,v;
                double t = objects.at(index)->findIntersection(r,u,v);
                if (t > 0.0 && t < tNear) {
                    tNear = t;
                    isectInfo.tNear = t;
                    isectInfo.hitObject = objects.at(index);
                    isectInfo.index = index;
                    isectInfo.u = u;
                    isectInfo.v = v;
                }
            }

            if (isectInfo.hitObject != nullptr) {
                vec3 intersection_point = r.pointAt(isectInfo.tNear);

                uint32_t vai[3]; // vertex attr index

                vai[0] = trisIndex[isectInfo.hitObject->getIndex() * 3];
                vai[1] = trisIndex[isectInfo.hitObject->getIndex() * 3 + 1];
                vai[2] = trisIndex[isectInfo.hitObject->getIndex() * 3 + 2];


                // vertex normal
                const vec3 &n0 = N[vai[0]];
                const vec3 &n1 = N[vai[1]];
                const vec3 &n2 = N[vai[2]];
                vec3 hitNormal = (1 - isectInfo.u - isectInfo.v) * n0 + isectInfo.u * n1 + isectInfo.v * n2;

                hitNormal = unit_vector(hitNormal);

                pixel_color = getColorAt(intersection_point, r.getRayDirection(), objects, isectInfo, hitNormal);
            }
            write_color(std::cout, pixel_color);
        }
        fprintf(stderr, "\r%3d%c", uint32_t(i / (float)image_height * 100), '%');
    }


    auto timeEnd = std::chrono::high_resolution_clock::now();
    auto passedTime = std::chrono::duration<double, std::milli>(timeEnd - timeStart).count();
    fprintf(stderr, "\rDone: %.2f (sec)\n", passedTime / 1000);


    delete [] P;
    delete [] N;
    delete [] points;
    delete [] normals;
    delete [] trisIndex;

    for (int i = 0; i < objects.size(); ++i) {
        delete objects.at(i);
    }


    std::cerr << "\nDone.\n";

}
