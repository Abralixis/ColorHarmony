#ifndef TOOLS_H
#define TOOLS_H

#include <vector>
#include <string>
#include <climits> // INT_MAX
#include "image_ppm.h"
#include "CompMean.h"
#include "Vec3color.h"
#include "Vec3HSL.h"

// Init && Action
void initAndApplyImage(std::string strPath, int mode, float r, float g, float b, float ColorSurfaceDegree);

// Save Img
void saveImage(std::string strOut, bool hasChanged, std::string strIn);

// Read PPM
void copyImg2TabPPM(OCTET *Img, Vec3Color **tab, int nW, int nH);

// Write PPM
void writeImgPPM(char *name, int nW, int nH, Vec3Color **tab);

// Conversion HSL
void arrayRGBtoHSL(Vec3Color **tabRGB, Vec3HSL **tabHSL, int nW, int nH);
void arrayHSLtoRGB(Vec3HSL **tabHSL, Vec3Color **tabRGB, int nW, int nH);

// Processing
void rotateAllByDegree(Vec3HSL **tabHSL, float degree, int nW, int nH);
void harmonize(Vec3HSL **tabHSL, int nW, int nH, std::vector<float> bands, float bandSize);
void computeAnalogous(Vec3HSL **imageInput, int nW, int nH, Vec3HSL mainColor, float degree);
void computeMonochromatic(Vec3HSL **imageInput, int nW, int nH, Vec3HSL mainColor);
void computeComplementary(Vec3HSL **imageInput, int nW, int nH, Vec3HSL mainColor, float degree);
void computeSplitComplementary(Vec3HSL **imageInput, int nW, int nH, Vec3HSL mainColor, float degree);
void computeTriadic(Vec3HSL **imageInput, int nW, int nH, Vec3HSL mainColor, float degree);
void computeSquare(Vec3HSL **imageInput, int nW, int nH, Vec3HSL mainColor, float degree);

// Analysis
void writeHistogram(const char *name, Vec3HSL **tabHSL, int nW, int nH);
void computeHistogram(Vec3HSL **tabHSL, int nW, int nH, int *tabHisto);

// Debug
void debugColor(Vec3HSL **tabHSL, int nW, int nH);

// k-means dominant color
Vec3Color kMeansDominantColor(std::string strPath, int nbIter, int nbCentroid);

// histo dominant color
int histoDominantColor(std::string strPath);

// Score
void ScorePSNR(Vec3Color **imgOrigin, Vec3Color **imgResult, int nW, int nH);
float ScoreColorHSL(Vec3HSL **tabHSL, int nW, int nH);

#endif