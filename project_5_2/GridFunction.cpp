#include"GridFunction.h"
#include<iostream>
using namespace std;

//implementation of class PDE
PDE::PDE(double A, double(*f)(double), double(*g)(double), double(*h)(double))
{
	a = A; IC = f; BC_L = g; BC_U = h;
}
double PDE::get_InitialValue(double x) { return IC(x); }
double PDE::get_LowerBoundaryValue(double t) { return BC_L(t); }
double PDE::get_UpperBoundaryValue(double t) { return BC_U(t); }
double PDE::get_constant() { return a;}

void PDE::displayPDE(double x, double t)
{
	cout << "My PDE:" << endl;
	cout << "IC(x) = " << IC(x) << endl;
	cout << "Lower BC(t) = " << BC_L(t) << endl;
	cout << "Upper BC(t) = " << BC_U(t) << endl;
}


//implementation of class GridFunction
//which specifically solves the PDE type of u_t=a u_xx
GridFunction::GridFunction()
{
	m = 1; n = 1; u = 0; t = 0;
}
GridFunction::GridFunction(double X_a, double X_b, double T1, int M, int N, PDE *U)
{
	m = M; n = N; u = U; t = 0; 
	x_a = X_a; x_b = X_b; T = T1;
	dx = (x_b - x_a) / n;
	dt = T / m;
	w_prev.resize(n + 1);
	w.resize(n + 1);
	for (int i = 0; i < n; i++) w[i] = u->get_InitialValue(dx*i);
}
GridFunction::~GridFunction()
{
	u = NULL; //do not delete the associated PDE object
}

void GridFunction::resetGridFunction(double X_a, double X_b, double T1, int M, int N, PDE *U)
{
	m = M; n = N; u = U; t = 0;
	x_a = X_a; x_b = X_b; T = T1;
	dx = (x_b - x_a) / n;
	dt = T / m;
	w_prev.resize(n + 1);
	w.resize(n + 1);
	for (int i = 0; i < n; i++) w[i] = u->get_InitialValue(dx*i);
}
void GridFunction::StepForward()
{
	t += dt;
	w_prev = w;
	w[0] = u->get_LowerBoundaryValue(t);
	w[n] = u->get_UpperBoundaryValue(t);

	for (int i = 1; i < n; i++)
	{
		w[i] = w_prev[i] + u->get_constant()*dt*(w_prev[i - 1] - 2 * w_prev[i] + w_prev[i + 1]) / (dx*dx);
	}
}

void GridFunction::solvePDE()
{
	for (int i = 0; i < m; i++) this->StepForward();
}

void GridFunction::displayGF()
{
	cout << "At current time T = " << T << endl;
	for (int i = 0; i <= n; i++) {
		cout << "w( " << dx*i << " , " << T << " ) = " << w[i] << endl;
	}
}

double GridFunction::getGridValue(int k)
{
	return w.at(k);
}
double GridFunction::calculateL2Error(double(*U)(double, double, double))
{
	double temp,err = 0;
	for (int i = 0; i <= n; i++) {
		temp = w[i] - U(i*dx, t, u->get_constant());
		err += temp*temp;
	}

	return sqrt(err / (n + 1));
}