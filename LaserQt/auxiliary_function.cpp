#include "auxiliary_function.h"

string GetOperatingSystemVersion() {
#ifdef __linux__
    return "Linux";
#endif
    return "Windows";
}

string GetWorkDirectory() {
    string _os = GetOperatingSystemVersion();
    string mainDirectory;
    if (_os == "Linux") {
        mainDirectory = "/home/summy/";
    } else {
        mainDirectory = "C://";
    }
    return mainDirectory;
}

string GetCurrentTime() {
    return "";
}

string GetCurrentDate() {
    return "";
}

pair<int, int> GetScreenSize() {
    pair<int, int> p = make_pair(1280, 640);
    return p;
}
