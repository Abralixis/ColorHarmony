#include "tools.h"

// Init && Action
void initAndApplyImage(std::string strPath, int mode, float r, float g, float b, float ColorSurfaceDegree)
{
    // CONVERT CHAR
    char *charPath = new char[strPath.length() + 1];
    strcpy(charPath, strPath.c_str());

    // IMG ALLOCATION
    int nH, nW, nTaille, nTaille3;
    OCTET *imgIn;
    readRowColPPM(charPath, &nH, &nW);
    nTaille = nH * nW;
    nTaille3 = nTaille * 3;
    arrayAllocation(imgIn, OCTET, nTaille3);
    readImagePPM(charPath, imgIn, nTaille);

    // ARRAY
    Vec3Color **imgOrigin = new Vec3Color *[nW];
    Vec3HSL **imgTransit = new Vec3HSL *[nW];
    Vec3Color **imgResult = new Vec3Color *[nW];
    for (int i = 0; i < nW; ++i)
    {
        imgOrigin[i] = new Vec3Color[nH];
        imgTransit[i] = new Vec3HSL[nH];
        imgResult[i] = new Vec3Color[nH];
    }

    // COPY
    copyImg2TabPPM(imgIn, imgOrigin, nH, nW);

    // CONVERT RGB->HSL
    arrayRGBtoHSL(imgOrigin, imgTransit, nH, nW);

    // GET INIT SCORE COLOR
    float scoreInit = ScoreColorHSL(imgTransit, nH, nW);

    // GET MAIN COLOR
    Vec3HSL mainColor = Vec3HSL();
    mainColor.convertRGBtoHSL(r, g, b);

    // MODE
    switch (mode)
    {
    case 0:
        printf(">>> Mode -> Analogous\n");
        computeAnalogous(imgTransit, nH, nW, mainColor, ColorSurfaceDegree);
        break;
    case 1:
        printf(">>> Mode -> Monochrome\n");
        computeMonochromatic(imgTransit, nH, nW, mainColor);
        break;
    case 2:
        printf(">>> Mode -> Complementary\n");
        computeComplementary(imgTransit, nH, nW, mainColor, ColorSurfaceDegree);
        break;
    case 3:
        printf(">>> Mode -> Adjacent Complementary\n");
        computeSplitComplementary(imgTransit, nH, nW, mainColor, ColorSurfaceDegree);
        break;
    case 4:
        printf(">>> Mode -> Triadic\n");
        computeTriadic(imgTransit, nH, nW, mainColor, ColorSurfaceDegree);
        break;
    case 5:
        printf(">>> Mode -> Square\n");
        computeSquare(imgTransit, nH, nW, mainColor, ColorSurfaceDegree);
        break;
    default:
        // Debug
        debugColor(imgTransit, nH, nW);
        // getDominantColorImage(imgTransit, nH, nW);
        break;
    }

    // CONVERT HSL->RGB
    arrayHSLtoRGB(imgTransit, imgResult, nH, nW);

    // GET FINAL SCORE COLOR
    float scoreFinal = ScoreColorHSL(imgTransit, nH, nW);
    printf("--- Score COLOR : %f --> %f\n", scoreInit, scoreFinal);

    // SCORE
    ScorePSNR(imgOrigin, imgResult, nH, nW);

    // WRITE
    writeImgPPM((char *)"output.ppm", nH, nW, imgResult);

    // CLEAN
    for (int i = 0; i < nW; i++)
    {
        delete[] imgOrigin[i];
        delete[] imgTransit[i];
        delete[] imgResult[i];
    }
    delete[] imgOrigin;
    delete[] imgTransit;
    delete[] imgResult;
    delete[] charPath;
    free(imgIn);
}

// Save Img
void saveImage(std::string strOut, bool hasChanged, std::string strIn)
{
    // INIT
    int nH, nW, nTaille, nTaille3;
    OCTET *ImgIn, *ImgOut;

    // DIRTY FLAG
    if (hasChanged)
    {
        // READ SIZE
        readRowColPPM((char *)"output.ppm", &nH, &nW);
        nTaille = nH * nW;
        nTaille3 = nTaille * 3;

        // ALLOCATION
        arrayAllocation(ImgIn, OCTET, nTaille3);
        readImagePPM((char *)"output.ppm", ImgIn, nTaille);
        arrayAllocation(ImgOut, OCTET, nTaille3);
    }
    else
    {
        // CONVERT CHAR
        char *charIn = new char[strIn.length() + 1];
        strcpy(charIn, strIn.c_str());

        // READ SIZE
        readRowColPPM(charIn, &nH, &nW);
        nTaille = nH * nW;
        nTaille3 = nTaille * 3;

        // ALLOCATION
        arrayAllocation(ImgIn, OCTET, nTaille3);
        readImagePPM(charIn, ImgIn, nTaille);
        arrayAllocation(ImgOut, OCTET, nTaille3);
    }

    // CONVERT CHAR
    char *charOut = new char[strOut.length() + 1];
    strcpy(charOut, strOut.c_str());

    // COPY
    for (int i = 0; i < nTaille3; i += 3)
    {
        ImgOut[i] = ImgIn[i];
        ImgOut[i + 1] = ImgIn[i + 1];
        ImgOut[i + 2] = ImgIn[i + 2];
    }

    // WRITE
    writeImagePPM(charOut, ImgOut, nH, nW);

    // CLEAN
    free(ImgIn);
    free(ImgOut);

    printf(">>> Image saved (ppm)\n");
}

// Read PPM
void copyImg2TabPPM(OCTET *Img, Vec3Color **tab, int nH, int nW)
{
    // SIZE
    int nTaille = nH * nW;
    int nTaille3 = nTaille * 3;

    // LECTURE COLOR
    int idx_i = 0;
    int idx_j = 0;
    for (int i = 0; i < nTaille3; i += 3)
    {
        tab[idx_i][idx_j] = Vec3Color(Img[i], Img[i + 1], Img[i + 2]);
        idx_i++;
        if (idx_i >= nW)
        {
            idx_i = 0;
            idx_j++;
        }
    }
    printf(">>> Image -> Array (ppm)\n");
}

// Write PPM
void writeImgPPM(char *name, int nH, int nW, Vec3Color **tab)
{
    // SIZE
    int nTaille = nH * nW;
    int nTaille3 = nTaille * 3;

    // IMG
    OCTET *Img;
    arrayAllocation(Img, OCTET, nTaille3);

    // ECRITURE
    for (int i = 0; i < nW; i++)
    {
        for (int j = 0; j < nH; j++)
        {
            Img[((j * nW + i) * 3)] = tab[i][j].getR();
            Img[((j * nW + i) * 3) + 1] = tab[i][j].getG();
            Img[((j * nW + i) * 3) + 2] = tab[i][j].getB();
        }
    }
    printf(">>> Write Image (ppm)\n");

    writeImagePPM(name, Img, nH, nW);
    free(Img);
}

// Conversion RGB->HSL
void arrayRGBtoHSL(Vec3Color **tabRGB, Vec3HSL **tabHSL, int nH, int nW)
{
    for (int i = 0; i < nW; i++)
    {
        for (int j = 0; j < nH; j++)
        {
            int r = tabRGB[i][j].getR();
            int g = tabRGB[i][j].getG();
            int b = tabRGB[i][j].getB();
            tabHSL[i][j].convertRGBtoHSL(r, g, b);
        }
    }
}

// Conversion HSL->RGB
void arrayHSLtoRGB(Vec3HSL **tabHSL, Vec3Color **tabRGB, int nH, int nW)
{
    for (int i = 0; i < nW; i++)
    {
        for (int j = 0; j < nH; j++)
        {
            float h = tabHSL[i][j].getH();
            float s = tabHSL[i][j].getS();
            float l = tabHSL[i][j].getL();
            tabRGB[i][j].convertHSLtoRGB(h, s, l);
        }
    }
}

// Wheel Rotation
void rotateAllByDegree(Vec3HSL **tabHSL, float degree, int nH, int nW)
{
    for (int i = 0; i < nW; i++)
    {
        for (int j = 0; j < nH; j++)
        {
            float color = tabHSL[i][j].getH() + degree;
            tabHSL[i][j].setH(color);
        }
    }
}

// Band computation
void harmonize(Vec3HSL **tabHSL, int nH, int nW, std::vector<float> bands, float bandSize)
{
    bool first = true;

    for (int i = 0; i < nW; i++)
    {
        for (int j = 0; j < nH; j++)
        {
            float color = tabHSL[i][j].getH();

            float desiredBand = bands[0];
            float signedDistance = Vec3HSL::distanceHue(color, desiredBand);
            float minDistance = signedDistance;
            if (minDistance < 0.)
            {
                minDistance *= -1;
            }

            float influenceSize = (float)(360. / bands.size());
            float radius = influenceSize / 2.;

            for (size_t i = 1; i < bands.size(); i++)
            {
                float signedDist = Vec3HSL::distanceHue(color, bands[i]);
                float dist = signedDist;
                if (dist < 0.)
                {
                    dist *= -1;
                }
                if (dist < minDistance)
                {
                    signedDistance = signedDist;
                    minDistance = dist;
                    desiredBand = Vec3HSL::fixHue(bands[i]);
                }
            }

            float outColor;

            outColor = (signedDistance / radius) * (bandSize / 2) + desiredBand;

            tabHSL[i][j].setH(outColor);
        }
    }
}

/** Histogram **/
void computeHistogram(Vec3HSL **tabHSL, int nH, int nW, int *tabHisto)
{
    for (int i = 0; i < nW; i++)
    {
        for (int j = 0; j < nH; j++)
        {
            tabHisto[(int)tabHSL[i][j].getH()]++;
        }
    }
}
void computeHistogramNoGrey(Vec3HSL **tabHSL, int nH, int nW, int *tabHisto)
{
    for (int i = 0; i < nW; i++)
    {
        for (int j = 0; j < nH; j++)
        {
            if (tabHSL[i][j].getS() != 0.)
            {
                tabHisto[(int)tabHSL[i][j].getH()]++;
            }
        }
    }
}
void writeHistogram(const char *name, Vec3HSL **tabHSL, int nH, int nW)
{
    int *tabHisto = new int[360]{0};
    computeHistogram(tabHSL, nH, nW, tabHisto);

    FILE *fp;
    fp = fopen((char *)name, "w+");

    if (fp == NULL)
    {
        printf("Error fopen!\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < 360; i++)
    {
        fprintf(fp, "%d %d\n", i, tabHisto[i]);
    }

    fclose(fp);

    printf(">>> Write Histo\n");

    delete[] tabHisto;
}
/** ********* **/

/** Harmony Mode **/
void computeAnalogous(Vec3HSL **imageInput, int nH, int nW, Vec3HSL mainColor, float degree)
{
    // Init
    float mainHue = mainColor.getH();

    // Calc band
    std::vector<float> band;
    band.push_back(mainHue);

    // Harmonize
    harmonize(imageInput, nH, nW, band, degree);
}

void computeMonochromatic(Vec3HSL **imageInput, int nH, int nW, Vec3HSL mainColor)
{
    // Init
    float mainHue = mainColor.getH();

    // Calc band
    std::vector<float> band;
    band.push_back(mainHue);

    // Harmonize
    harmonize(imageInput, nH, nW, band, 0);
}

void computeComplementary(Vec3HSL **imageInput, int nH, int nW, Vec3HSL mainColor, float degree)
{
    // Init
    float mainHue = mainColor.getH();

    // Calc band
    std::vector<float> band;
    band.push_back(mainHue);
    band.push_back(mainHue + 180.0);

    // Harmonize
    harmonize(imageInput, nH, nW, band, degree / 2.0);
}
void computeSplitComplementary(Vec3HSL **imageInput, int nH, int nW, Vec3HSL mainColor, float degree)
{
    // Init
    float mainHue = mainColor.getH();

    // Calc band
    std::vector<float> band;
    band.push_back(mainHue);
    band.push_back(mainHue + 150.0);
    band.push_back(mainHue + 210.0);

    // Harmonize
    harmonize(imageInput, nH, nW, band, degree / 3.0);
}

void computeTriadic(Vec3HSL **imageInput, int nH, int nW, Vec3HSL mainColor, float degree)
{
    // Init
    float mainHue = mainColor.getH();

    // Calc band
    std::vector<float> band;
    band.push_back(mainHue);
    band.push_back(mainHue + 120.0);
    band.push_back(mainHue + 240.0);

    // Harmonize
    harmonize(imageInput, nH, nW, band, degree / 3.0);
}
void computeSquare(Vec3HSL **imageInput, int nH, int nW, Vec3HSL mainColor, float degree)
{
    // Init
    float mainHue = mainColor.getH();

    // Calc band
    std::vector<float> band;
    band.push_back(mainHue);
    band.push_back(mainHue + 90.0);
    band.push_back(mainHue + 180.0);
    band.push_back(mainHue + 270.0);

    // Harmonize
    harmonize(imageInput, nH, nW, band, degree / 4.0);
}
/** ************ **/

// Debug Hue
void debugColor(Vec3HSL **image, int nH, int nW)
{
    for (int i = 0; i < nW; i++)
    {
        for (int j = 0; j < nH; j++)
        {
            image[i][j].setS(1.);
            image[i][j].setL(0.5);
        }
    }
}

// k-means dominant color
Vec3Color kMeansDominantColor(std::string strPath, int nbIter, int nbCentroid)
{
    // CONVERT CHAR
    char *charPath = new char[strPath.length() + 1];
    strcpy(charPath, strPath.c_str());

    // IMG ALLOCATION
    int nH, nW, nTaille, nTaille3;
    OCTET *imgIn;
    readRowColPPM(charPath, &nH, &nW);
    nTaille = nH * nW;
    nTaille3 = nTaille * 3;
    arrayAllocation(imgIn, OCTET, nTaille3);
    readImagePPM(charPath, imgIn, nTaille);

    // ARRAY
    Vec3Color **imageRGB = new Vec3Color *[nW];
    for (int i = 0; i < nW; ++i)
    {
        imageRGB[i] = new Vec3Color[nH];
    }

    // COPY
    copyImg2TabPPM(imgIn, imageRGB, nH, nW);

    // Init
    srand(time(0));
    std::vector<CompMean> kMeanVect(nbCentroid);

    // Choose random centroid
    for (int i = 0; i < nbCentroid; ++i)
    {
        kMeanVect[i].setCentroid(imageRGB[rand() % nW][rand() % nH]);
    }

    // Convergence
    // For each iterations
    for (int nbI = 0; nbI < nbIter; ++nbI)
    {
        // Clear Vectors
        for (int clear = 0; clear < nbCentroid; ++clear)
        {
            kMeanVect[clear].clearEnsList();
        }

        // For each Pixel
        for (int i = 0; i < nW; i++)
        {
            for (int j = 0; j < nH; j++)
            {
                // Target
                Vec3Color pxTarget = imageRGB[i][j];

                // Dist Min for each CompMean
                int idxComp = 0;
                double distMin = INT_MAX;
                for (int i = 0; i < nbCentroid; ++i)
                {
                    double d = kMeanVect[i].getCentroid().distVecColor(pxTarget);
                    if (distMin > d)
                    {
                        distMin = d;
                        idxComp = i;
                    }
                }

                // Attribution pxTarget
                kMeanVect[idxComp].addEnsList(pxTarget);
            }
        }

        // Compute Centroids (average)
        for (int c = 0; c < nbCentroid; c++)
        {
            kMeanVect[c].computeCentroid();
        }
    }

    // Return the centroid with ensList size max
    int idxMax = 0;
    int nbEnsMax = kMeanVect[0].getEnsList().size();
    for (int i = 0; i < nbCentroid; i++)
    {
        if (nbEnsMax < kMeanVect[i].getEnsList().size())
        {
            idxMax = i;
            nbEnsMax = kMeanVect[i].getEnsList().size();
        }
    }

    /* DEBUG IMAGE !
    Vec3Color **tabRGB = new Vec3Color *[nW];
    for (int i = 0; i < nW; ++i)
    {
        tabRGB[i] = new Vec3Color[nH];
    }

        // For each Pixel
        for (int i = 0; i < nW; i++)
        {
            for (int j = 0; j < nH; j++)
            {
                // Target
                Vec3Color pxTarget = imageRGB[i][j];

                // Dist Min for each CompMean
                int idxComp = 0;
                double distMin = INT_MAX;
                for (int i = 0; i < nbCentroid; ++i)
                {
                    double d = kMeanVect[i].getCentroid().distVecColor(pxTarget);
                    if(distMin > d)
                    {
                        distMin = d;
                        idxComp = i;
                    }
                }

                // Attribution pxTarget
                tabRGB[i][j].setRGB(kMeanVect[idxComp].getCentroid().getR(), kMeanVect[idxComp].getCentroid().getG(), kMeanVect[idxComp].getCentroid().getB());
            }
        }

    // Write Image debug.
    writeImgPPM((char *)"output/kMeanDebug.ppm", nH, nW, tabRGB);
    */

    // CLEAN
    for (int i = 0; i < nW; i++)
    {
        delete[] imageRGB[i];
    }
    delete[] imageRGB;
    delete[] charPath;
    free(imgIn);

    printf(">>> Compute K-Mean\n");

    return kMeanVect[idxMax].getCentroid();
}

// Histo dominant color
int histoDominantColor(std::string strPath)
{
    // CONVERT CHAR
    char *charPath = new char[strPath.length() + 1];
    strcpy(charPath, strPath.c_str());

    // IMG ALLOCATION
    int nH, nW, nTaille, nTaille3;
    OCTET *imgIn;
    readRowColPPM(charPath, &nH, &nW);
    nTaille = nH * nW;
    nTaille3 = nTaille * 3;
    arrayAllocation(imgIn, OCTET, nTaille3);
    readImagePPM(charPath, imgIn, nTaille);

    // ARRAY
    Vec3Color **imageRGB = new Vec3Color *[nW];
    Vec3HSL **imageHSL = new Vec3HSL *[nW];
    for (int i = 0; i < nW; ++i)
    {
        imageRGB[i] = new Vec3Color[nH];
        imageHSL[i] = new Vec3HSL[nH];
    }

    // DATA
    int *tabHisto = new int[360]{0};

    // COPY
    copyImg2TabPPM(imgIn, imageRGB, nH, nW);

    // CONVERT RGB->HSL
    arrayRGBtoHSL(imageRGB, imageHSL, nH, nW);

    // Compute Histogram and seek maximum
    computeHistogramNoGrey(imageHSL, nH, nW, tabHisto);
    int maxHisto = 0;
    int hue = 0;
    for (int i = 0; i < 360; i++)
    {
        if (tabHisto[i] > maxHisto)
        {
            maxHisto = tabHisto[i];
            hue = i;
        }
    }

    // DEBUG
    // writeHistogram((char *)"output/data_histoNoGrey.txt", imageHSL, nW, nH);

    // CLEAN
    for (int i = 0; i < nW; i++)
    {
        delete[] imageRGB[i];
        delete[] imageHSL[i];
    }
    delete[] imageRGB;
    delete[] imageHSL;
    delete[] charPath;
    free(imgIn);

    printf(">>> Compute Histo\n");

    return hue;
}

// SCORE PSNR
void ScorePSNR(Vec3Color **imgOrigin, Vec3Color **imgResult, int nH, int nW)
{
    double PSNR = 0.f;
    double EQM = 0;

    for (int i = 0; i < nW; i++)
    {
        for (int j = 0; j < nH; j++)
        {
            EQM += pow(imgOrigin[i][j].distVecColor(imgResult[i][j]), 2);
        }
    }

    EQM /= (nW * nH) * 3;
    PSNR = 10 * log10(pow(255, 2) / EQM);
    printf("--- Score PSNR : %f\n", PSNR);
}

// SCORE PSNR
float ScoreColorHSL(Vec3HSL **tabHSL, int nH, int nW)
{
    // Histo calc
    int *tabHisto = new int[360]{0};
    for (int i = 0; i < nW; i++)
    {
        for (int j = 0; j < nH; j++)
        {
            tabHisto[(int)tabHSL[i][j].getH()]++;
        }
    }

    // For each val == 0
    float score = 0;
    for (int i = 0; i < 360; i++)
    {
        if (tabHisto[i] == 0)
        {
            score++;
        }
    }

    // Clean
    delete[] tabHisto;

    return score /= 3.59;
}