#include "CompMean.h"

// Construct
CompMean::CompMean()
{
    this->centroid = Vec3Color(0, 0, 0);
}
CompMean::CompMean(Vec3Color v)
{
    this->centroid = v;
}

// ensList
void CompMean::clearEnsList()
{
    this->ensList.clear();
}
void CompMean::addEnsList(Vec3Color v)
{
    this->ensList.push_back(v);
}
std::vector<Vec3Color> CompMean::getEnsList()
{
    return this->ensList;
}

// Centroid
Vec3Color CompMean::getCentroid()
{
    return this->centroid;
}
void CompMean::setCentroid(Vec3Color v)
{
    this->centroid = v;
}

// Compute
void CompMean::computeCentroid()
{
    int sizeEns = this->ensList.size();
    if (sizeEns == 0)
    {
        this->centroid = Vec3Color(0, 0, 0);
    }
    else
    {
        Vec3Color res(0, 0, 0);
        for (int i = 0; i < sizeEns; i++)
        {
            res += this->ensList[i];
        }
        res /= sizeEns;
        this->centroid = res;
    }
}