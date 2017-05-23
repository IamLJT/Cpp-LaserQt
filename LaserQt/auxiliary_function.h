#ifndef AUXILIARY_FUNCTION_H
#define AUXILIARY_FUNCTION_H

#include <string>

using namespace std;

string GetOperatingSystemVersion();
string GetWorkDirectory();
string GetCurrentTime();
string GetCurrentDate();
pair<int, int> GetScreenSize();

#endif // AUXILIARY_FUNCTION_H
