g++ -O3 -std=c++11 -c -o Filter.o Filter.cpp -DBUILD_DLL -ID:/boost/include -LD:/boost/lib
g++ -O3 -std=c++11 -c -o GridDivide.o GridDivide.cpp -DBUILD_DLL -ID:/boost/include -LD:/boost/lib
g++ -O3 -std=c++11 -c -o icp.o icp.cpp -DBUILD_DLL -ID:/boost/include -LD:/boost/lib
g++ -O3 -std=c++11 -c -o icpPointToPlane.o icpPointToPlane.cpp -DBUILD_DLL -ID:/boost/include -LD:/boost/lib
g++ -O3 -std=c++11 -c -o icpPointToPoint.o icpPointToPoint.cpp -DBUILD_DLL -ID:/boost/include -LD:/boost/lib
g++ -O3 -std=c++11 -c -o kd_tree.o kd_tree.cpp -DBUILD_DLL -ID:/boost/include -LD:/boost/lib
g++ -O3 -std=c++11 -c -o matrix.o matrix.cpp -DBUILD_DLL -ID:/boost/include -LD:/boost/lib
g++ -O3 -std=c++11 -c -o PointCloudAlgorithm.o PointCloudAlgorithm.cpp -DBUILD_DLL -ID:/boost/include -LD:/boost/lib
g++ -O3 -std=c++11 -c -o readfile.o readfile.cpp -DBUILD_DLL -ID:/boost/include -LD:/boost/lib

g++ -shared -o libalgorithm.dll Filter.o GridDivide.o icp.o icpPointToPlane.o icpPointToPoint.o kd_tree.o matrix.o PointCloudAlgorithm.o readfile.o
move libalgorithm.dll ../../winlib/algorithm

del Filter.o
del GridDivide.o
del icp.o
del icpPointToPlane.o
del icpPointToPoint.o
del kd_tree.o matrix.o
del PointCloudAlgorithm.o
del readfile.o

pause
