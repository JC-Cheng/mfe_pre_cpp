#include <iostream>
#include <cmath>
#include "DataSeries.h"
using namespace std;

//main program
int main() {
	int n = 1000;
	DataSeries Data(n);
	int i;

	for (i = 1; i <= n; i++) {
		Data.setValue(sin(i*0.01) + cos(i*0.02) + i*0.005*cos(i*0.5), i - 1);
	}

	cout << "AbsVar = " << Data.AbsoluteVariation() << endl;
	cout << "Using naive method, Q =  " << Data.CountChangeAboveThreshold_N3(0.8) << endl;
	cout << "Using improved method, Q = " << Data.CountChangeAboveThreshold(0.8) << endl;
	cout << "Using TA's method, Q = " << Data.CountChangeAboveThreshold_NLgN(0.8) << endl;

	system("pause");
	return 0;
}