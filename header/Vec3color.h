#ifndef VEC_COLOR_H
#define VEC_COLOR_H

#include <iostream>
#include <cmath>

class Vec3Color
{
private:
    int R, G, B;

public:
    // CONSTRUCTOR
    Vec3Color();
    Vec3Color(int R_in, int G_in, int B_in);

    // GETTER
    int getR();
    int getG();
    int getB();

    // SETTER
    void setR(int a);
    void setG(int a);
    void setB(int a);
    void setRGB(int a, int b, int c);

    // DIST
    double distVecColor(Vec3Color v);

    // CONVERSION
    void convertHSLtoRGB(float H, float S, float L);

    // OPERATOR
    Vec3Color &operator+=(const Vec3Color &v);
    Vec3Color &operator/=(int a);
    friend bool operator==(Vec3Color const &v1, Vec3Color const &v2);
    friend std::ostream &operator<<(std::ostream &os, const Vec3Color &v);
};

#endif