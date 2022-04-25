#ifndef VEC_HSL_H
#define VEC_HSL_H

#include <iostream>  // cout
#include <algorithm> // min, max
#include "Vec3color.h"

class Vec3HSL
{
private:
    float H, S, L;

public:
    // CONSTRUCTOR
    Vec3HSL();
    Vec3HSL(float H_in, float S_in, float L_in);

    // GETTER
    float getH();
    float getS();
    float getL();

    // SETTER
    void setH(float a);
    void setS(float a);
    void setL(float a);
    void setHSL(float a, float b, float c);

    // CONVERSION
    void convertRGBtoHSL(int r_in, int g_in, int b_in);

    static float distanceHue(float h1, float h2);
    static float fixHue(float hue);

    friend std::ostream &operator<<(std::ostream &os, const Vec3HSL &v);
};

#endif