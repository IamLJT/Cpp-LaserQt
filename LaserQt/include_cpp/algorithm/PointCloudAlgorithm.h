#include <direct.h>
#include <iostream>
#include <windows.h>
#include "Filter.h"
#include "icpPointToPoint.h"
#include "icpPointToPlane.h"
#include "readfile.h"
#include "GridDivide.h"

int PointCloudKThreshlod(const char * Path);
int PointCloudFitting(const char * inPath, bool isFilter, const char * TargetData);
