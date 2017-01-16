#include<iostream>
#include<cmath>
#include<vector>
#include<fstream>
#include"GridFunction.h"

double f(double x); //Boundary condition
double g(double t); //Lower initial condition
double h(double t); //Upper initial condition
double u(double x, double t, double a); //analytic solution of PDE

const double PI = 2 * asin(1);
const double precision = pow(2, -30);

int main() {

	PDE myPDE(0.25, f, g, h);
	GridFunction *W = new GridFunction[6];
	
	int ans;
	cout << "Enter 1 to test order of convergence, 2 to test stabilities, and 0 to exit: ";
	cin >> ans;

	while (ans)
	{
		if (ans == 1) //test order of convergence
		{
			ofstream file_c("Convergence.csv");
			double e;
			int N[6] = { 50,100,200,400,800,1000 };
			for (int i = 0; i < 6; i++) {

				W[i].resetGridFunction(0, 1, 0.75, N[i] * N[i], N[i], &myPDE);
				W[i].solvePDE();
				e = W[i].calculateL2Error(u);
				
				cout << "N = " << N[i] << ", L2 Error = " << e << endl;
				file_c << N[i] << "," << e << endl;
				
			}
		}

		if (ans == 2) //fixed N, test stability properties
		{
			ofstream file_s("Stability.csv");
			double e;
			int N[3] = { 250,500,750 };
			double k[6] = { 2.5,2.55,2.6,2.65,2.7,2.75 };
			for (int j = 0; j < 3; j++) {
				for (int i = 0; i < 6; i++) {

					W[i].resetGridFunction(0, 1, 0.75, ceil(N[j] * N[j] / k[i]), N[j], &myPDE);
					W[i].solvePDE();
					e = W[i].calculateL2Error(u);
					cout << "N = " << N[j] << ", k = " << k[i] << ", M = " << ceil(N[j] * N[j] / k[i]) << ", L2 Error = " << e << endl;
					file_s << N[j] << "," << k[i] << "," << ceil(N[j] * N[j] / k[i]) << "," << e << endl;
				}
				cout << endl;
				file_s << endl;
			}
		}

		cout << "Enter 1 to test order of convergence, 2 to test stabilities, and 0 to exit: ";
		cin >> ans;

	}
	

	system("pause");
	return 0;
}


//property of funcitons
double f(double x) { return 1 - 2 * abs(x - 0.5); }
double g(double t) { return 0; }
double h(double t) { return 0; }
double u(double x, double t, double a)
{
	double temp, y, ans = 0;
	int k = 1;

	do
	{
		y = 2*k - 1;
		temp = exp(-a*PI*PI*y*y*t)*cos(PI*y*(x - 0.5)) / (y*y);
		ans += temp; k++;
	}
	while (temp > precision);

	return ans * 8 / (PI*PI);
}