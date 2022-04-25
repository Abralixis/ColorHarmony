#include "Vec3HSL.h"

// CONSTRUCTOR
Vec3HSL::Vec3HSL()
{
    this->H = 0; // Hue  (0-360)
    this->S = 0; // Saturation (0-1)
    this->L = 0; // this->L (0-1)
}
Vec3HSL::Vec3HSL(float H_in, float S_in, float L_in)
{
    if (H_in >= 0 && H_in < 360 && S_in >= 0 && S_in <= 1 && L_in >= 0 && L_in <= 1)
    {
        this->H = H_in;
        this->S = S_in;
        this->L = L_in;
    }
    else
    {
        printf("ERREUR : Vec3HSL a des valeurs incorrectes\n");
    }
}

// GETTER
float Vec3HSL::getH()
{
    return this->H;
}
float Vec3HSL::getS()
{
    return this->S;
}
float Vec3HSL::getL()
{
    return this->L;
}

// SETTER
void Vec3HSL::setH(float hue)
{
    this->H = Vec3HSL::fixHue(hue);
}
void Vec3HSL::setS(float a)
{
    this->S = a;
}
void Vec3HSL::setL(float a)
{
    this->L = a;
}
void Vec3HSL::setHSL(float a, float b, float c)
{
    this->H = a;
    this->S = b;
    this->L = c;
}

std::ostream &operator<<(std::ostream &os, const Vec3HSL &v)
{
    os << "(" << v.H << "," << v.S << ',' << v.L << ")";
    return os;
}

// CONVERSION
void Vec3HSL::convertRGBtoHSL(int r_in, int g_in, int b_in)
{
    float r = (float)r_in / 255.;
    float g = (float)g_in / 255.;
    float b = (float)b_in / 255.;

    float Xmax = std::max(r, std::max(g, b));
    float Xmin = std::min(r, std::min(g, b));

    float chromaRange = Xmax - Xmin;
    float hue;
    if (chromaRange == 0.)
    {
        hue = 0.;
    }
    else
    {
        if (Xmax == r)
        {
            hue = 60. * (0. + (g - b) / chromaRange);
        }
        if (Xmax == g)
        {
            hue = 60. * (2. + (b - r) / chromaRange);
        }
        if (Xmax == b)
        {
            hue = 60. * (4. + (r - g) / chromaRange);
        }
    }

    while (hue < 0.)
    {
        hue += 360.;
    }
    while (hue >= 360.)
    {
        hue -= 360.;
    }

    this->H = hue;
    this->L = (Xmax + Xmin) / 2.;

    if (this->L == 0. || this->L == 1.)
    {
        this->S = 0;
    }
    else
    {
        this->S = chromaRange / (1 - abs(2 * Xmax - chromaRange - 1));
    }
}

float Vec3HSL::distanceHue(float start, float target)
{

    float distance = start - target;
    float distanceTs = (start - 360.) - target;
    float distancetS = (start + 360.) - target;

    float AbsDistance = distance;
    if (AbsDistance < 0.)
    {
        AbsDistance *= -1;
    }
    float AbsDistanceTs = distanceTs;
    if (AbsDistanceTs < 0.)
    {
        AbsDistanceTs *= -1;
    }
    float AbsDistancetS = distancetS;
    if (AbsDistancetS < 0.)
    {
        AbsDistancetS *= -1;
    }

    if (AbsDistance < AbsDistanceTs)
    {
        if (AbsDistance < AbsDistancetS)
        {
            return distance;
        }
        return distancetS;
    }
    else
    {
        if (AbsDistanceTs < AbsDistancetS)
        {
            return distanceTs;
        }
        return distancetS;
    }
    return distance;
}

float Vec3HSL::fixHue(float hue)
{
    while (hue >= 360.)
    {
        hue -= 360.;
    }
    while (hue < 0.)
    {
        hue += 360.;
    }
    return hue;
}