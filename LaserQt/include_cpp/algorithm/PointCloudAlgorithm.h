#ifndef POINTCLOUDALGORITHM_H
#define POINTCLOUDALGORITHM_H

#include <direct.h>
#include <iostream>
#include <windows.h>
#include "Filter.h"
#include "icpPointToPoint.h"
#include "icpPointToPlane.h"
#include "readfile.h"
#include "GridDivide.h"
#include "float.h"



class Pointcloudalgorithm {
public:
    int PointCloudKThreshlod(const char* Path);
    void PointCloudFitting(const char *inPath, bool isFilter, const char *TargetData);
};

#endif // POINTCLOUDALGORITHM_H
