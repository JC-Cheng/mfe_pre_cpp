#include <cassert>
#include <cmath>
#include "DataSeries.h"
using namespace std;

int AboveThreshold(double &x, double &y, double &T);
int SubSeq_ChgAboveT(double *x, int m, double &T);

//constructor
DataSeries::DataSeries(int k) {
	dLen = k;
	data = new double[k];
}

//destructor
DataSeries::~DataSeries() {
	delete[] data;
}

double DataSeries::getValue(int k) {
	assert(k >= 0 && k < dLen);
	return data[k];
}

void DataSeries::setValue(double x, int k) {
	assert(k >= 0 && k < dLen);
	data[k] = x;
}

double DataSeries::AbsoluteVariation() {
	double sum = 0;
	for (int i = 0; i < dLen - 1; i++) sum += abs(data[i + 1] - data[i]);
	return sum;
}

int DataSeries::CountChangeAboveThreshold_N3(double T) {

	int **count = new int*[dLen];
	int i, j, k, Q;

	//declare a n*n array and give its boundary condition
	for (i = 0; i < dLen; i++) {
		count[i] = new int[dLen];
		if (i < dLen - 1) count[i][i + 1] = AboveThreshold(data[i], data[i + 1], T);
	}

	for (i = 2; i < dLen; i++) {
		for (j = 0; j < dLen - i; j++) {
			Q = AboveThreshold(data[j], data[j + i], T);
			for (k = j + 1; k < j + i; k++) {
				if (Q < count[j][k] + count[k][j + i]) Q = count[j][k] + count[k][j + i];
			}
			count[j][j + i] = Q;
		}
	}
	Q = count[0][dLen - 1];

	//give back memory
	for (i = 0; i < dLen; i++) delete[] count[i];
	delete[] count;

	return Q;
}

//a better solutions!
int DataSeries::CountChangeAboveThreshold(double T) {
	return SubSeq_ChgAboveT(data, dLen, T);
}

int SubSeq_ChgAboveT(double *x, int m, double &T) {

	//size m >= 2
	if (m <= 1 || T < 0) return 0;

	//find the smallest p such that abs(x[p - 1] - x[p]) >= T ; stop = 1 means we do find the p before the series end
	int p = 0, stop = 0;
	while (p < m - 1) {
		p++;
		if (abs(x[p - 1] - x[p]) >= T) {
		//if (AboveThreshold(x[p - 1], x[p], T)) {
			stop = 1;
			break;
		}
	}

	if (p == 1 && p < m)
	//abs(x[0] - x[1]) >= T
	//We can reduce the size of problem by 1.
	{
		return stop + SubSeq_ChgAboveT(x + 1, m - 1, T);
	}
	else
	//abs(x[i - 1] - x[i]) < T, for all i from 1 to k - 1.
	//It's non-trivial! We have to look into subseq x[0], ..., x[k - 1].
	{
		int **count = new int*[p];
		int Q, i, j, k;

		//declare a k*k array and give its boundary condition
		for (i = 0; i < p; i++) {
			count[i] = new int[p];
			if (i < p - 1) count[i][i + 1] = 0;
		}

		for (i = 2; i < p; i++) {
			for (j = 0; j < p - i; j++) {
				Q = AboveThreshold(x[j], x[j + i], T);
				for (k = j + 1; k < j + i; k++) {
					if (Q < count[j][k] + count[k][j + i]) Q = count[j][k] + count[k][j + i];
				}
				count[j][j + i] = Q;
			}
		}
		Q = count[0][p - 1];

		//give back memories
		for (i = 0; i < p; i++) delete[] count[i];
		delete[] count;

		return Q + stop + SubSeq_ChgAboveT(x + p, m - p, T);
	}
}

int DataSeries::CountChangeAboveThreshold_NLgN(double T) {

	int chg = 0;
	int *MinThresh = new int[dLen];

	for (int i = 0; i < dLen; i++) MinThresh[i] = dLen;

	for (int i = 0; i < dLen - 1; i++) {
		for (int j = i + 1; j < dLen; j++) {
			if (AboveThreshold(data[i], data[j],T)) {
				MinThresh[i] = j;
				break;
			}
		}
	}

	int start = 0;
	while (start < dLen) {

		int k = dLen;
		int found = 0;
		for (int i = start; i < dLen; i++) {
			if (MinThresh[i] < k) {
				k = MinThresh[i];
				found = 1;
			}
		}
		start = k;
		chg += found;
	}

	delete[] MinThresh;
	return chg;
}

//determine difference above threshold
int AboveThreshold(double &x, double &y, double &T) {
	return (abs(x - y) >= T) ? 1 : 0;
}