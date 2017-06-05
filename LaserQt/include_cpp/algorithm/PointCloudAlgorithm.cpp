#include "PointCloudAlgorithm.h"

using namespace std;

#ifndef _MAX_
	#define MAX 256
#endif

int PointCloudKThreshlod(const char * Path) {
	
	int32_t dim = 3, num0 = 0;
    double * M = ReadFile(Path, num0, dim);

	griddivide_new Grid_new0(M, num0, dim);
	Grid_new0.gridpoint(M, 0.05);
	int count = 0;
	// ��һ����������
	double* M_new = Grid_new0.grid_filter(M, count, 5, 2);
	delete [] M;
	// ��kmeans����
	griddivide_new Grid_new(M_new, count, dim);
	vector<vector<double>> temp_cluster = Grid_new.grid_kmeans(M_new, 2, count);

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

	int num_G = 0; // �µĵ���
	griddivide Grid_temp(M_cluster, num, dim);
	Grid_temp.dividenum(10, 10, 20);
	Grid_temp.grid_point(M_cluster, num, dim);
	double* M_e0 = Grid_temp.first_filter_grid(M_cluster, 
		num_G, dim, Grid_temp.GetInterval());


	char sPath[MAX] = "E:\\TempData.txt";
    WriteFile(sPath, M_e0, num_G, dim);
	
	delete [] M_e0;
	delete [] M_cluster;
	delete [] M_new;

    return num-num_G;
	//return 10000 * M_new[count * dim - 1];
	//return count;
}

void PointCloudFitting(const char *inPath, bool isFilter, const char *TargetData) {
	char Path[MAX] = "E:\\TempData.txt";
	int32_t dim = 3, num = 0, num0 = 0;
	double *M;
	double *T = ReadFile(TargetData, num, dim);
	if(false == isFilter)
		M = ReadFile(inPath, num0, dim);
	else
		M = ReadFile(Path, num0, dim);
	// start with identity as initial transformation
	// in practice you might want to use some kind of prediction here
	Matrix R = Matrix::eye(3);
	Matrix t(3,1);

	// run point-to-plane ICP (-1 = no outlier threshold)
	cout << endl << "Running ICP (point-to-plane, no outliers)" << endl;
	IcpPointToPoint icp(T, num, dim);
	icp.fit(M,num0,R,t,-1);
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

	char OutPath[MAX] = "E:\\FittingData.txt";

	WriteFile(OutPath, M0, num, dim);
	
	delete [] T;
	delete [] M;
}
