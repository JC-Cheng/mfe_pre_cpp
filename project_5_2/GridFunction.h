#ifndef __GRIDFUNCTION_H__
#define __GRIDFUNCTION_H__

#include<vector>
using namespace std;

//a general type of IBVP PDE u(x,t)
class PDE
{
public:

	PDE(double A, double(*f)(double), double(*g)(double), double(*h)(double));
	
	double get_InitialValue(double x);
	double get_LowerBoundaryValue(double t);
	double get_UpperBoundaryValue(double t);
	double get_constant();

	void displayPDE(double x, double t);

private:

	double a; //the constant governing PDE
	double(*IC)(double); //initial condition [x_a,x_b] -> R
	double(*BC_L)(double); //lower boundary condition: R+ -> R
	double(*BC_U)(double); //upper boundary condition  R+ -> R

};

//declaration of GridFunction class
//which specifically solves the PDE type of u_t=a u_xx for (x,t) in [x_a, x_b]*R+
class GridFunction
{
public:
	//constructor
	GridFunction();
	GridFunction(double X_a, double X_b, double T1, int M, int N, PDE *U);
	~GridFunction();
	
	void resetGridFunction(double X_a, double X_b, double T1, int M, int N, PDE *U);
	void StepForward(); //step forward in time by dt
	void solvePDE(); //solve the PDE, get U(x,T1)
	void displayGF();

	double getGridValue(int k);
	double calculateL2Error(double(*U)(double, double, double));

private:
	
	PDE *u;
	double x_a, x_b, T;
	int m; //# of interval in x dimension
	int n; //# of interval in t dimension
	vector<double> w_prev, w; //vector representing grid points at a given time (we have n+1 points)
	double dx, dt; //interval length
	double t; //current time t
};


#endif