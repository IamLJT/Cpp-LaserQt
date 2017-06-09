#include "PointCloudAlgorithm.h"

using namespace std;

#ifndef _MAX_
	#define MAX 256
#endif

int PointCloudKThreshlod(const char * Path) {
	
	int32_t dim = 3, num0 = 0;
	// 读取扫描文件，isread记录读取是否成功
	// 读取格式为%lf,%lf,%lf
	bool isread = true;
    double * M = ReadFile(Path, num0, dim, 1, isread);
	// 防止因为文件读取失败而导致系统崩溃
	if (!isread) return -1;
	
	// 简化网格，如果数据量很大，那么就将grid_filter中
	// 的mode参数设置为1，如果数据量不大的话就设置为2，
	// 只是做一个简单的去噪，只是去掉远离点云的点
	griddivide_new Grid_new0(M, num0, dim);
	Grid_new0.gridpoint(M, 0.05);
	int count = 0;
	// 进行滤躁或简化
	double* M_new = Grid_new0.grid_filter(M, count, 5, 2);
	delete [] M;
	// 如果count为0，说明网格划分的尺寸不合适，把所有
	// 的点都给去掉了，默认每个网格内至少5个点
	if (count == 0) return -2;
	
	// 进行kmeans分类，默认是2类
	griddivide_new Grid_new(M_new, count, dim);
	vector<vector<double>> temp_cluster = 
		Grid_new.grid_kmeans(M_new, 2, count);
	// 获取目标点云的类的数据
	double sum1_cluster = 0, sum2_cluster = 0;
	int num = temp_cluster[0].size() / dim;
	for (int i = 0; i < num; ++i)
		sum1_cluster += temp_cluster[0][i * dim + 2];
	sum1_cluster /= num;
	num = temp_cluster[1].size() / dim;
	for (int i = 0; i < num; ++i)
		sum2_cluster += temp_cluster[1][i * dim + 2];
	sum2_cluster /= num;
	int index = 0;
	if (sum1_cluster > sum2_cluster) index = 0;
	else index = 1;
	num = temp_cluster[index].size() / dim;
	double *M_cluster = new double[num * dim];
	int k = 0;
	for (auto c : temp_cluster[index])
		M_cluster[k++] = c;

	// 重新进行网格划分，是为了进一步去噪
	int num_G = 0;
	griddivide Grid_temp(M_cluster, num, dim);
	Grid_temp.dividenum(15, 15, 20);
	Grid_temp.grid_point(M_cluster, num, dim);
	double* M_e0 = Grid_temp.first_filter_grid(M_cluster, 
		num_G, dim, Grid_temp.GetInterval());
	// 如果去噪后点云数据num_G为0，则说明网格有问题，与上面划分时问题相同
	if (num_G == 0) return -3;

	char sPath[MAX] = "E:\\TempData.txt";
    WriteFile(sPath, M_e0, num_G, dim);
	
	// 删除动态申请的内存
	delete [] M_e0;
	delete [] M_cluster;
	delete [] M_new;
	// 计算从聚类后的数据去噪的点
    return num-num_G;
}

int PointCloudFitting(const char *inPath, bool isFilter, const char *TargetData) {
	char Path[MAX] = "E:\\TempData.txt";
	int32_t dim = 3, num = 0, num0 = 0;
	double *M;
	bool isread = true;
	double *T = ReadFile(TargetData, num, dim, 1, isread);
	if (!isread) return -1;
	if(false == isFilter)
		M = ReadFile(inPath, num0, dim, 1, isread);
	else
		M = ReadFile(Path, num0, dim, 1, isread);
	if (!isread) return -2;
	
	// 初始化旋转和平移矩阵
	Matrix R = Matrix::eye(3);
	Matrix t(3,1);

	// 运行点对点ICP算法，-1表示没有阈值
	cout << endl << "Running ICP (point-to-plane, no outliers)" << endl;
	
	// T表示目标矩阵，fit函数求出旋转矩阵R和平移矩阵t
	IcpPointToPoint icp(T, num, dim);
	icp.fit(M,num0,R,t,-1);

	// 获取旋转后的点云数据
	Matrix mx = Matrix::ArrayToMatrix(M, num, dim);

	double *M0 = new double[num * dim];
	double r00 = R.val[0][0]; double r01 = R.val[0][1]; double r02 = R.val[0][2];
	double r10 = R.val[1][0]; double r11 = R.val[1][1]; double r12 = R.val[1][2];
	double r20 = R.val[2][0]; double r21 = R.val[2][1]; double r22 = R.val[2][2];
	double t0  = t.val[0][0]; double t1  = t.val[1][0]; double t2  = t.val[2][0];

	for(int idx=0; idx<num; idx++)
	{
		M0[idx*3+0] = r00*mx.val[idx][0] + r01*mx.val[idx][1] + r02*mx.val[idx][2] + t0;
		M0[idx*3+1] = r10*mx.val[idx][0] + r11*mx.val[idx][1] + r12*mx.val[idx][2] + t1;
		M0[idx*3+2] = r20*mx.val[idx][0] + r21*mx.val[idx][1] + r22*mx.val[idx][2] + t2;
	}

	// 保存输出数据
	char OutPath[MAX] = "E:\\FittingData.txt";
	WriteFile(OutPath, M0, num, dim);
	
	// 删除动态申请的内存
	delete [] T;
	delete [] M;
	
	return 0;
}
