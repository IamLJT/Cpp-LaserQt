#ifndef READFILE_H
#define READFILE_H

#pragma warning(disable:4996)

#include <fstream>
#include <vector>
#include "matrix.h"
#include "float.h"
#include  <direct.h>

void ReadxyzFile(const char* Path);
double* ReadFile(const char* strPath, int& num_p, int& dim_p, int mode, bool& result);
void WriteFile(char* Path, double* M, int32_t num, int32_t dim);

#endif