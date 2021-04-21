#include <vector>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <stdio.h>
#include "constant.h"

using namespace std;

vector<sphere> allSpheres;

void parse(string inputFile);

color shoot(ray& r, const point3& light) {
    double t;
    // What if two spheres intersect with ray at same point? -> Choose closest t value.
    bool hit = false;
    sphere hitObject;
    int hitIndex = 0;
    double closestT = kInfinity;
    for (int i = 0; i < allSpheres.size() ; ++i) {
        if (r.intersect(allSpheres[i],t) && t < closestT) {
            closestT = t;
            hitObject = allSpheres[i];
            hitIndex = i;
            hit = true;
        }
    }

    /*
     * If there is an intersection point with any sphere, send shadow ray from this point,
     * otherwise, return background color.
     */
    if (hit) {
        allSpheres.erase(allSpheres.begin() + hitIndex);
        color c = hitObject.color();
        vec3 pi = r.at(closestT);   // intersection point and is it in shadow or is it blocked any objects ?
        ray shadRay(pi, light-pi);    // a ray from intersection point to light point.
        bool pointIsBlocked = false;
        double tt;
        for (sphere s: allSpheres) {
            if (shadRay.intersect(s,tt)){
                pointIsBlocked = true;
            }
        }
        allSpheres.push_back(hitObject);

        /*
         * If intersection point blocked from another object, color will multiply by 0.1,
         * otherwise, it returns its own color.
         */

        if (!pointIsBlocked) {

            vec3 normal = hitObject.getNormal(pi);
            vec3 l = light - pi;
            double dt = dot(normalize(normal), normalize(l));
            if (dt > 0) return c;  // points are in light area.
            else return c - (c * dt * -1);
            // else return c * 0.1;    // points are in shadow.

        }
        // shadow due to other objects.
        return { 25,25,25};

    }

    // background color
    vec3 unit_direction = normalize(r.direction());
    auto a = 0.5*(unit_direction.y() + 1.0);
    return ((1.0-a)*color(255, 255, 255) + a*color(0, 0, 0)) * 0.3;
}

int main(int argc, char** argv){

    if (argc < 2) {
        printf("Missing input file path -> raytracer.o path of input file");
        return 0;
    }

    string inputFile = argv[1];
    parse(inputFile);

    // Image

    const double aspect_ratio = 1.0;
    const int image_width = 1000;
    const int image_height = static_cast<int>(image_width / aspect_ratio);


    // Camera

    const double viewport_width = 100.0;   // --> -50 to 50
    const double viewport_height = viewport_width / aspect_ratio;
    const double focal_length = 100.0;  // distance between eye and screen.

    const point3 origin = point3(0,0,0);  // eye point
    const point3 light = point3(500,500,500);   // light point

    const point3 horizontal = vec3(viewport_width, 0, 0);
    const point3 vertical = vec3(0, viewport_height, 0);
    const point3 upper_left_corner = origin - horizontal / 2 + vertical / 2 + vec3(0, 0, focal_length);


    // Render

    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for (int i = 0; i < image_height; ++i) {
        std::cerr << "\rScanning lines done: " << i << ' ' << std::flush;
        for (int j = 0; j < image_width; ++j) {
            double u = double (j + 0.5) / (image_width);
            double v = double (i + 0.5) / (image_height);
            ray r(origin, upper_left_corner + u*horizontal - v*vertical - origin);
            color pixel_color = shoot(r, light);
            clamp255(pixel_color);
            write_color(std::cout, pixel_color);
        }
    }

    std::cerr << "\nDone.\n";
}

void parse(string inputFile){
    ifstream file(inputFile);

    string numberOfSpheres;
    getline(file,numberOfSpheres);

    for (int i = 0; i < stoi(numberOfSpheres); ++i) {
        string colorLine, positionLine, radiusLine;
        getline(file, colorLine);
        stringstream col(colorLine);
        string c;
        vector<double> colors;
        while (col >> c) {
            colors.push_back(stod(c));
        }
        getline(file, positionLine);
        stringstream pos(positionLine);
        string p;
        vector<double> positions;
        while (pos >> p) {
            positions.push_back(stod(p));
        }
        getline(file,radiusLine);
        sphere newSphere = sphere(
                point3(positions[0],positions[1],positions[2]),
                stod(radiusLine),
                color(colors[0],colors[1],colors[2]));
        allSpheres.push_back(newSphere);
    }
}