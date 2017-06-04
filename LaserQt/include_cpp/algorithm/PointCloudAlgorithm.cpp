#include "PointCloudAlgorithm.h"

using namespace std;

#ifndef _MAX_
	#define MAX 256
#endif

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
    switch (ul_reason_for_call) {
		case DLL_PROCESS_ATTACH:
			printf("DLL_PROCESS_ATTACH\n");
			break;
		case DLL_THREAD_ATTACH:
			printf("DLL_THREAD_ATTACH\n");
			break;
		case DLL_THREAD_DETACH:
			printf("DLL_THREAD_DETACH\n");
			break;
		case DLL_PROCESS_DETACH:
			printf("DLL_PROCESS_DETACH\n");
			break;
	}
    return TRUE;
}

int PointCloudKThreshlod(const char * Path) {

    printf("进入滤波程序\n");
	int32_t dim = 3, num = 0, m = 0, n = 0;
	vector<int> DataFile(4, 0);
	//char p[] = "C:\\Users\\SummyChou\\Desktop\\src.txt";
    double * M = ReadFile(Path, DataFile);

    printf("ReadFile成功%s\n", Path);

	num = DataFile[0];
	dim = DataFile[1];
	m = DataFile[2];
	n = DataFile[3];

	griddivide_new Grid_new0(M, num, dim);
	Grid_new0.gridpoint(M, 0.02);
	int count = 0;
	// ��һ����������
	double* M_new = Grid_new0.grid_filter(M, count, 5, 1);

	// ��kmeans����
	griddivide_new Grid_new(M_new, count, dim);
	vector<vector<double>> temp_cluster = Grid_new.grid_kmeans(M_new, 2, count);

	num = temp_cluster[0].size() / dim;
	double * M_cluster1 = new double[num * dim];
	int k = 0;
	for (auto c : temp_cluster[0])
		M_cluster1[k++] = c;
	num = temp_cluster[1].size() / dim;
	double * M_cluster2 = new double[num * dim];
	k = 0;
	for (auto c : temp_cluster[1])
		M_cluster2[k++] = c;

	int index = 0;
	if (temp_cluster[0].size() > temp_cluster[1].size())
		index = 1;
	else
		index = 0;
	num = temp_cluster[index].size() / dim;
	double *M_cluster = new double[num * dim];
	k = 0;
	for (auto c : temp_cluster[index])
		M_cluster[k++] = c;

        printf("分类成功\n");

        /*Filter flr(M, num, dim, m, n);

        double *M0 = flr.ThresholdFilter(20);*/

	int num_G = 0; // �µĵ���
	griddivide Grid_temp(M_cluster, num, dim);
	Grid_temp.dividenum(15, 15, 20);
	Grid_temp.grid_point(M_cluster, num, dim);
	double* M_e0 = Grid_temp.first_filter_grid(M_cluster, num_G, dim, 0.6 * Grid_temp.GetInterval());

    //    printf("网格去噪成功\n");

	char sPath[MAX] = "D:\\TempData.txt";
	//getcwd(sPath, MAX_PATH);

    //printf("获取当前路径\n");
	//strcat(sPath, "\\cache\\TempData.txt");
        WriteFile(sPath, M_e0, num_G, dim);

        return num-num_G;

        return 0;
}

void PointCloudFitting(const char *inPath, bool isFilter, const char *TargetData) {
	char Path[MAX] = "D:\\TempData.txt";
	//getcwd(Path, MAX_PATH);
	//strcat(Path, "D:\\TempData.txt");

	int32_t dim = 3, num = 0, m = 0, n = 0;
	vector<int> DataFile(4, 0);
	double *M;
	double *T = ReadFile(TargetData, DataFile);
	if(false == isFilter)
		M = ReadFile(inPath, DataFile);
	else
		M = ReadFile(Path, DataFile);

	num = DataFile[0];
	dim = DataFile[1];
	m = DataFile[2];
	n = DataFile[3];
	// start with identity as initial transformation
	// in practice you might want to use some kind of prediction here
	Matrix R = Matrix::eye(3);
	Matrix t(3,1);

	// run point-to-plane ICP (-1 = no outlier threshold)
	cout << endl << "Running ICP (point-to-plane, no outliers)" << endl;
	IcpPointToPoint icp(T, num, dim);
	icp.fit(M,num,R,t,-1);
	//	T is dest-matrix, num is number of data
	//	R and t means Ratate and Translate Matrix

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

	char OutPath[MAX] = "D:\\FittingData.txt";
	//getcwd(OutPath, MAX_PATH);
	//strcat(OutPath, "D:\\FittingData.txt");

	//cout << mx.m << endl;

	WriteFile(OutPath, M0, num, dim);
}
