#ifndef COMP_MEAN_H
#define COMP_MEAN_H

#include <vector>      /* vector */
#include <cmath>       /* pow, sqrt */
#include "Vec3color.h" /* Color */

class CompMean
{
private:
    Vec3Color centroid;
    std::vector<Vec3Color> ensList;

public:
    // Construct && Destruct
    CompMean();
    CompMean(Vec3Color v);

    // ensList
    void clearEnsList();
    void addEnsList(Vec3Color v);
    std::vector<Vec3Color> getEnsList();

    // Centroid
    Vec3Color getCentroid();
    void setCentroid(Vec3Color v);

    // Compute
    void computeCentroid();
};

#endif