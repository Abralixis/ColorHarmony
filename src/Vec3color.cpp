#include "Vec3color.h"

// CONSTRUCTOR
Vec3Color::Vec3Color()
{
    this->R = 0;
    this->G = 0;
    this->B = 0;
}
Vec3Color::Vec3Color(int R_in, int G_in, int B_in)
{
    if (R_in >= 0 && R_in < 256 && G_in >= 0 && G_in < 256 && B_in >= 0 && B_in < 256)
    {
        this->R = R_in;
        this->G = G_in;
        this->B = B_in;
    }
    else
    {
        printf("ERREUR : Vec3Color a des valeurs incorrectes\n");
    }
}

// GETTER
int Vec3Color::getR()
{
    return this->R;
}
int Vec3Color::getG()
{
    return this->G;
}
int Vec3Color::getB()
{
    return this->B;
}

// SETTER
void Vec3Color::setR(int a)
{
    this->R = a;
}
void Vec3Color::setG(int a)
{
    this->G = a;
}
void Vec3Color::setB(int a)
{
    this->B = a;
}
void Vec3Color::setRGB(int a, int b, int c)
{
    this->R = a;
    this->G = b;
    this->B = c;
}

// DIST
double Vec3Color::distVecColor(Vec3Color v)
{
    double powR = pow(this->R - v.getR(), 2);
    double powG = pow(this->G - v.getG(), 2);
    double powB = pow(this->B - v.getB(), 2);
    return sqrt(powR + powG + powB);
}

// OPERATOR
Vec3Color &Vec3Color::operator/=(const int a)
{
    if (a != 0)
    { // Attention si classe = 0
        this->R /= a;
        this->G /= a;
        this->B /= a;
    }
    return *this;
}
Vec3Color &Vec3Color::operator+=(const Vec3Color &v)
{
    this->R += v.R;
    this->G += v.G;
    this->B += v.B;
    return *this;
}
bool operator==(Vec3Color const &v1, Vec3Color const &v2)
{
    return (v1.R == v2.R && v1.G == v2.G && v1.B == v2.B);
}
std::ostream &operator<<(std::ostream &os, const Vec3Color &v)
{
    os << "(" << v.R << "," << v.G << ',' << v.B << ")";
    return os;
}

// CONVERSION
void Vec3Color::convertHSLtoRGB(float H, float S, float L)
{
    float chroma = (1 - fabs(2 * L - 1)) * S;
    float Hp = H / 60.;
    float X = chroma * (1 - fabs(fmod(Hp, 2) - 1.));
    float r, g, b;

    if (Hp < 1.)
    {
        r = chroma;
        g = X;
        b = 0.;
    }
    else
    {
        if (Hp < 2.)
        {
            r = X;
            g = chroma;
            b = 0.;
        }
        else
        {
            if (Hp < 3.)
            {
                r = 0.;
                g = chroma;
                b = X;
            }
            else
            {
                if (Hp < 4.)
                {
                    r = 0.;
                    g = X;
                    b = chroma;
                }
                else
                {
                    if (Hp < 5.)
                    {
                        r = X;
                        g = 0.;
                        b = chroma;
                    }
                    else
                    {
                        r = chroma;
                        g = 0.;
                        b = X;
                    }
                }
            }
        }
    }
    float m = L - (chroma / 2.);

    this->R = round((r + m) * 255.);
    this->G = round((g + m) * 255.);
    this->B = round((b + m) * 255.);
}