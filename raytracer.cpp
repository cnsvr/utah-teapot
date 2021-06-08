#include <vector>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <stdio.h>
#include "constant.h"
#define MAX_DEPTH 10


using namespace std;

void parse(string inputFile, vector<object*> &objects, color &ambientColorOfScene);

void write_color(std::ostream &out, color pixel_color) {
    out << static_cast<int>(pixel_color.getColorRed() * 255) << ' '
        << static_cast<int>(pixel_color.getColorGreen() * 255) << ' '
        << static_cast<int>(pixel_color.getColorBlue() * 255) << '\n';
}


/*
 * Calculates reflection direction.
 */
vec3 reflect(const vec3 &I, const vec3 &N)
{
    return I - 2 * dot(I, N) * N;
}

/*
 * Returns the closest object index with respect to value.
 */
int closestObjectIndex(vector<double> intersections) {
    int index_of_min_t;

    if (intersections.size() == 0) {
        return -1;
    } else if (intersections.size() == 1) {
        if (intersections.at(0) > 0) {
            return 0; // first element
        } else {
            return -1;
        }
    } else {
        double max = 0;
        for (int i = 0; i < intersections.size(); i++) {
            if (max < intersections.at(i)) {
                max = intersections.at(i);
            }
        }

        if (max > 0) {
            // we only want positive intersections
            for (int index = 0; index < intersections.size(); index++) {
                if (intersections.at(index) > 0 && intersections.at(index) <= max) {
                    max = intersections.at(index);
                    index_of_min_t = index;
                }
            }

            return index_of_min_t;
        }
        else {
            // all the intersections were negative
            return -1;
        }
    }
}


/*
 * Returns final color of pixel consist of ambient color and diffuse color.
 */

color getColorAt(vec3 intersection_point, vec3 intersection_ray_direction, vector<object*> objects, int index_of_closest_object, int &depthCount, light scene_light) {
    color object_ambient_color = objects.at(index_of_closest_object)->getAmbientColor();
    vec3 object_normal = objects.at(index_of_closest_object)->getNormalAt(intersection_point);

    // Ground pattern
    if (objects.at(index_of_closest_object)->getType() == PLANE) {
        int square = (int)floor(intersection_point.x()) + (int)floor(intersection_point.z());

        if (square % 2 == 0) {
            object_ambient_color.setColorRed(0.5);
            object_ambient_color.setColorGreen(0.5);
            object_ambient_color.setColorBlue(0.5);
        } else {
            object_ambient_color.setColorRed(0);
            object_ambient_color.setColorGreen(0.25);
            object_ambient_color.setColorBlue(0);
        }
    }

    color final_color;

    color diffuseColor = objects.at(index_of_closest_object)->getDiffuseColor();

    vec3 light_dir = unit_vector(scene_light.getLightPosition() - intersection_point);

    double cosine_angle = dot(object_normal, light_dir);

    // If cosine angle is 0, light and object normal is perpendicular, so light source doesn't come
    // this intersection point.
    if (cosine_angle > 0) {

        // If any objects block a light source coming the point, shadow occurs.
        bool shadow = false;
        double distance = light_dir.length();


        ray shadow_ray(intersection_point,light_dir);

        vector<double> secondary_intersections;

        for (int object_index = 0; object_index < objects.size(); object_index++) {
            secondary_intersections.push_back(objects.at(object_index)->findIntersection(shadow_ray));
        }
        for (int c = 0; c < secondary_intersections.size(); c++) {
            if (secondary_intersections.at(c) > 0.000001) {
                 if (secondary_intersections.at(c) <= distance) {
                    shadow = true;
                    break;
                 }

            }
        }
        if (!shadow) {
            diffuseColor = diffuseColor.multiply(scene_light.getLightColor()).scalar(cosine_angle);
        }else {
            diffuseColor = object_ambient_color.scalar(-0.5);
        }
    } else {
        diffuseColor = diffuseColor.multiply(scene_light.getLightColor()).scalar(0);
    }

    final_color = object_ambient_color.add(diffuseColor);


    // Reflection starts here
    if (objects.at(index_of_closest_object)->getType() == SPHERE) {
        vec3 reflection_direction = reflect(intersection_ray_direction, object_normal);

        ray reflection_ray(intersection_point, reflection_direction);

        vector<double> reflection_intersections;

        for (int reflection_index = 0; reflection_index < objects.size(); reflection_index++) {
            reflection_intersections.push_back(objects.at(reflection_index)->findIntersection(reflection_ray));
        }
        int index_of_closest_object_with_reflection = closestObjectIndex(reflection_intersections);

        /*
         * If no reflection is left or depth reached the maximum depth, it returns final color ;
         * otherwise, continues with calculating new local color.
        */
        if (index_of_closest_object_with_reflection != -1 && depthCount < MAX_DEPTH) {
            if (reflection_intersections.at(index_of_closest_object_with_reflection) >  0.0000001) {
                vec3 reflection_intersection_point = reflection_ray.pointAt(reflection_intersections.at(index_of_closest_object_with_reflection));
                vec3 reflection_intersection_ray_direction = reflection_direction;
                depthCount = depthCount + 1;
                color reflection_color = getColorAt(reflection_intersection_point, reflection_intersection_ray_direction,objects, index_of_closest_object_with_reflection, depthCount, scene_light);
                final_color = final_color.add(reflection_color);
            }
        }
    }

    return final_color;

}

int main(int argc, char** argv){


    if (argc < 2) {
        printf("Missing input file path -> raytracer.o path of input file");
        return 0;
    }

    vector<object*> objects;
    color ambientColorOfScene;
    string inputFile = argv[1];

    parse(inputFile,objects,ambientColorOfScene);

    // Image

    const double aspect_ratio = 1.0;
    const int image_width = 1000;
    const int image_height = static_cast<int>(image_width / aspect_ratio);


    // Camera

    const double viewport_width = 100.0;   // --> -50 to 50
    const double viewport_height = viewport_width / aspect_ratio;
    const double focal_length = 100.0;  // distance between eye and screen.

    const vec3 origin = vec3(0,0,0);  // eye point

    const vec3 horizontal = vec3(viewport_width, 0, 0);
    const vec3 vertical = vec3(0, viewport_height, 0);
    const vec3 upper_left_corner = origin - horizontal / 2 + vertical / 2 + vec3(0, 0, focal_length);


    // Light and Ground Plane
    vec3 light_position (500,500,500);
    light scene_light (light_position, color(1,1,1));

    plane* ground = new plane(PLANE, vec3(0,1,0),1,color(1,1,1));
    objects.push_back(dynamic_cast<object*>(ground));


    // Render


    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for (int i = 0; i < image_height; ++i) {
        std::cerr << "\rScanning lines done: " << i << ' ' << std::flush;
        for (int j = 0; j < image_width; ++j) {
            double u = double (j + 0.5) / (image_width);
            double v = double (i + 0.5) / (image_height);
            ray r(origin, upper_left_corner + u*horizontal - v*vertical - origin);

            color pixel_color = ambientColorOfScene;   // Default scene color

            int depthCount = 0;

            vector<double> intersections;
            for (int index = 0; index < objects.size(); index++) {
                intersections.push_back(objects.at(index)->findIntersection(r));
            }

            int index_of_closest_object = closestObjectIndex(intersections);

            // If there is an intersection, get ambient + diffuse color.
            if (index_of_closest_object != -1) {
                vec3 intersection_point = r.pointAt(intersections.at(index_of_closest_object));
                pixel_color = getColorAt(intersection_point,r.getRayDirection(), objects, index_of_closest_object, depthCount,scene_light);
            }

            if (depthCount > 0) {
                double count = 1.0 / (depthCount);
                pixel_color = pixel_color.scalar(count);
            }

            pixel_color.clamp();

            write_color(std::cout, pixel_color);
        }
    }

    for (int i = 0; i < objects.size(); ++i) {
        delete objects.at(i);
    }

    std::cerr << "\nDone.\n";
}


void parse(string inputFile, vector<object*> &objects, color &ambientColorOfScene) {
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
        getline(file, radiusLine);

        sphere* newSphere = new sphere(SPHERE,
                                       vec3(positions[0], positions[1], positions[2]),
                                       stod(radiusLine),
                                       color(colors[0], colors[1], colors[2]).scalar(1.0/255.0),
                                       color(colors[3], colors[4], colors[5]).scalar(1.0/255.0));
        objects.push_back(dynamic_cast<object*>(newSphere));
    }

    string ambientOfScene;
    getline(file, ambientOfScene);
    stringstream colorsOfAmbientOfScene(ambientOfScene);
    string s;
    vector<double> colorOfScene;
    while (colorsOfAmbientOfScene >> s) {
        colorOfScene.push_back(stod(s));
    }

    ambientColorOfScene.setColorRed(colorOfScene[0]);
    ambientColorOfScene.setColorGreen(colorOfScene[1]);
    ambientColorOfScene.setColorBlue(colorOfScene[2]);
    ambientColorOfScene = ambientColorOfScene.scalar(1.0/255.0);
}