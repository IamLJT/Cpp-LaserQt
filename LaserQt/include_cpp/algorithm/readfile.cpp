#include "readfile.h"
#include <vector>
#include <algorithm>

#ifndef _MAX_
#define MAX 256
#endif

struct id {
	id() { m = n = 0; }
	int m, n;
};

// 一个存储三个点坐标的结构体
struct Point3d {
	Point3d(){ x = y = z = 0.0f; }
	float x, y, z;
	// 重载<运算符，方便后面排序
	bool operator< (const Point3d& p2) const {
		if(x == p2.x) {
			if(y == p2.y)
				return (z < p2.z);
			else
				return (y < p2.y);
		}
		else
			return (x < p2.x);
	}
};

int cmp(Point3d p1, Point3d p2) {
	return p1 < p2;
}

//	读取.xyz文件
void ReadxyzFile(const char* Path) {
	FILE *fp = fopen(Path, "r");
	if(fp == NULL) {
		printf("Cannot open point cloud file.\n");
		return;
	}

	//	获取行数，即点的数量，用于设置std::vector的容量
	const int BUFSIZE = 512;
	char buf[BUFSIZE];
	int rowNumber = 0;
	while(fgets(buf, BUFSIZE, fp) != NULL) {	//	行读取 
		++ rowNumber;
	}
	fclose(fp);
	fp = 0;

	//	重新打开文件
	fp = fopen(Path, "r");
	id pointID;
	Point3d pointXYZ;
	Point3d pointColor;

	std::vector<id> vecPtID;
	std::vector<Point3d> vecPtXYZ;		//	点坐标
	std::vector<Point3d> vecPtColor;	//	颜色

	vecPtID.reserve(rowNumber);
	vecPtXYZ.reserve(rowNumber);
	vecPtColor.reserve(rowNumber);

	//	数据读取
	while(fgets(buf, BUFSIZE, fp) != NULL) {
		// 数据格式严格按照每8个数的形式进行读取
		sscanf(buf, "%f %f %f %f %f %f %f %f",
			&(pointID.m),    &(pointID.n),
			&(pointXYZ.x),   &(pointXYZ.y),   &(pointXYZ.z),
			&(pointColor.x), &(pointColor.y), &(pointColor.z));

		vecPtID.push_back(pointID);
		vecPtXYZ.push_back(pointXYZ);
		vecPtColor.push_back(pointColor);
	}

	fclose(fp);
	fp = 0;

	sort(vecPtXYZ.begin(), vecPtXYZ.end(), cmp);

	double *M = new double[vecPtXYZ.size() * 3];
	for(int i=0; i<(int)vecPtXYZ.size(); i++) {
		M[i*3]   = vecPtXYZ[i].x;
		M[i*3+1] = vecPtXYZ[i].y;
		M[i*3+2] = vecPtXYZ[i].z;
	}
	char sPath[MAX];
	// 获取当前文件夹
	getcwd(sPath, MAX_PATH);

	strcat(sPath, "\\提取数据.txt");
	FILE *ifp = fopen(sPath, "w+");	//	以写文件的方式打开，再关闭即是清空文件
	fclose(ifp);
	// 保存文件
	WriteFile(sPath, M, rowNumber, 3);
	delete [] M;
}

// 读取普通的txt
double* ReadFile(const char* strPath, int& num_p, int& dim_p, int mode)	 {
	FILE *fp;
	fp = fopen(strPath, "r");
	std::vector<double> vec;
	int num = 0, dim = 3;
	double idata1, idata2, idata3;
	if (fp != NULL) {
		printf("success to open file\n");
		while(!feof(fp)) {
			if (mode == 1) //	读取txt文件，格式是每行x,y,z\n或者x,y,z
				fscanf(fp, "%lf,%lf,%lf", &idata1, &idata2, &idata3);
			else if (mode == 2) //	读取txt文件，格式是每行x y z\n或者x y z
				fscanf(fp, "%lf %lf %lf", &idata1, &idata2, &idata3);
			vec.push_back(idata1);
			vec.push_back(idata2);
			vec.push_back(idata3);
			num++;
		}
	}
	else {
		printf("failed to open file\n");
	}
	
	num_p = num;
	dim_p = dim;
	// 重新保存到double类型的数组中
	double *M = new double[(int)vec.size()];
	for(int i=0; i<(int)vec.size(); i++)
		M[i]=vec[i];
	// 清理工作
	vec.clear(); fclose(fp);
	return M;
}

void WriteFile(char* strPath, double* M, int32_t num, int32_t dim)
{
	FILE *fp;
	fp = fopen(strPath, "w+");
	if (fp != NULL) {
		printf("success to open writefile\n");
	for(int i=0; i<num*dim; i+=dim) {
		// 最后一行不需要换行
		if(i<(num-1)*dim)
			fprintf(fp, "%lf,%lf,%lf\n", M[i], M[i+1], M[i+2]);
		else
			fprintf(fp, "%lf,%lf,%lf", M[i], M[i+1], M[i+2]);
	}
	}
	else 
		printf("failed to open writefile\n");
	fclose(fp);
}