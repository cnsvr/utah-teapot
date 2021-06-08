//
// Created by Furkan C. on 17.04.2021.
//

#ifndef COLOR_H
#define COLOR_H

#include <iostream>
#include "vec3.h"

class color {
private:
    double red, green, blue;
public:
    color () {}
    color (double r, double g, double b) : red(r), green(g), blue(b) {}

    double getColorRed() { return red; }
    double getColorGreen() { return green; }
    double getColorBlue() { return blue; }

    void setColorRed(double redValue) { red = redValue; }
    void setColorGreen(double greenValue) { green = greenValue; }
    void setColorBlue(double blueValue) { blue = blueValue; }

    double brightness () { return (red + green + blue) / 3; }

    color add(color c) { return color(red + c.red, green + c.green, blue + c.blue); }
    color multiply(color c) { return color(red * c.red, green * c.green, blue * c.blue); }
    color scalar(double s) { return color(red * s, green * s, blue * s); }
    color average(color c) { return color(red * c.red / 2, green * c.green / 2, blue * c.blue / 2); }
    color clamp() {
        double allLight = red + green + blue;
        double excessLight = allLight - 3;
        if (excessLight > 0) {
            red = red + excessLight * (red / allLight);
            green = green + excessLight * (green / allLight);
            blue = blue + excessLight * (blue / allLight);
        }

        if (red > 1) { red = 1; }
        if (red < 0) { red = 0; }
        if (blue > 1) { blue = 1; }
        if (blue < 0) { blue = 0; }
        if (green > 1) { green = 1; }
        if (green < 0) { green = 0; }

        return color(red, green, blue);

    }
};

inline std::ostream& operator<<(std::ostream &out, color &c) {
    return out << c.getColorRed() << ' ' << c.getColorGreen() << ' ' << c.getColorBlue();
}

#endif

