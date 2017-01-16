// find the root of f(x)
#include <iostream>
#include <iomanip>
#include <cmath>
using namespace std;

const double precision = pow(10, -17);

double f(double x, double a, double b, double c)
{
	return sin((a*x) / (1 + x*x) + 1)*atan(b*x - 0.5) + exp(-c*x)*atan(x);
}

bool MyEqual(double v1, double v2) //define "equal" in this program; we allow slight error
{
	if (abs(v1 - v2) < precision) { return true; }
	else { return false; }
}

double RootFinderSMNew(double f(double, double, double, double), double a, double b, double c, double A, double B, int N);
double RootFinderBNew(double f(double, double, double, double), double a, double b, double c, double A, double B, int N);
double RootFinderRidder(double f(double, double, double, double), double a, double b, double c, double A, double B, int N);

int main()
{
	double B_root, S_root, R_root;
	double a = 4.5, b = 2.8, c = 1, A=-1, B=8;
	int N = 30;

	cout << "Give function parameters, a, b & c:" << endl;
	cin >> a >> b >> c;

	cout << "Give initial values, A & B:" << endl;
	cin >> A >> B;

	cout << "Give number of iterations, N:" << endl;
	cin >> N;

	B_root = RootFinderBNew(f, a, b, c, A, B, N);
	S_root = RootFinderSMNew(f, a, b, c, A, B, N);
	R_root = RootFinderRidder(f, a, b, c, A, B, N);
	
	cout << "Bisection: x = " << B_root << " ; f(x) = " << f(B_root, a, b, c) << endl;
	cout << "Secant   : x = " << S_root << " ; f(x) = " << f(S_root, a, b, c) << endl;
	cout << "Riddler  : x = " << R_root << " ; f(x) = " << f(R_root, a, b, c) << endl;

	system("pause");
	return 0;
}

//Bisection Method:
double RootFinderBNew(double f(double, double, double, double), double a, double b, double c, double A, double B, int N)
{
	double f_left = f(A, a, b, c), f_right = f(B, a, b, c);
	if (MyEqual(f_left, 0)) return A;
	if (MyEqual(f_right, 0)) return B;
	if (f_left*f_right > 0) {
		cout << "Invalid interval [" << A << "," << B << "]: please select again!";
		return 0;
	}

	double mid, f_mid;
	//cout << "Bisection Method:" << endl;
	for (int i = 0; i < N; i++) {
		mid = 0.5*(A + B);
		f_mid = f(mid, a, b, c);
		//cout << "i = " << i + 1 << " ; x_i = " << mid << endl;
		if (MyEqual(A, B)) { break; }
		if ((f_left*f_mid) < 0) { B = mid; f_right = f_mid; }
		if ((f_right*f_mid) < 0) { A = mid; f_left = f_mid; }
	}

	return  0.5*(A + B);
}

//Secant Method:
double RootFinderSMNew(double f(double, double, double, double), double a, double b, double c, double A, double B, int N)
{
	//n for new, p for previous
	double xn = A, xp = B, fxn = f(A, a, b, c), fxp = f(B, a, b, c), temp;
	if (MyEqual(fxn, 0)) return xn;
	if (MyEqual(fxp, 0)) return xp;

	//cout << "Secant Method:" << endl;
	for (int i = 0; i < N; i++) {
		temp = xn;
		if (MyEqual(fxn, fxp)) break;
		xn = xn - fxn*(xn - xp) / (fxn - fxp);
		xp = temp;

		fxp = fxn;
		fxn = f(xn, a, b, c);

		//cout << "i = " << i + 1 << " ; x_i = " << xn << endl;
	}

	return xn;
}

//Ridder Method:
double RootFinderRidder(double f(double, double, double, double), double a, double b, double c, double A, double B, int N)
{
	double fxm = f(A, a, b, c), fxp = f(B, a, b, c), fxs, xm = A, xp = B, xs, fz, z;
	if (MyEqual(fxm, 0)) return xm;
	if (MyEqual(fxp, 0)) return xp;
	
	if (fxm < 0 && fxp > 0) {
		//well-picked xm & xp
	}
	else if (fxm > 0 && fxp < 0) {
		fxs = fxm; fxm = fxp; fxp = fxs;
		xs = xm; xm = xp; xp = xs;
	}
	else {
		cout << "Invalid initial values; Please pick A, B again!" << endl;
		return 0;
	}

	double temp;
	//cout << "Ridder Method:" << endl;
	for (int i = 0; i < N; i++) {
		z = (xm + xp)*0.5;
		fz = f(z, a, b, c);
		xs = z - (z - xm) / sqrt(fz*fz - fxm*fxp)*fz;
		fxs = f(xs, a, b, c);

		if (fxs > 0) { fxp = fxs; xp = xs; }
		else if(fxs < 0) { fxm = fxs; xm = xs; }
		else return xs;

		//cout << "i = " << i + 1 << " ; x_i = " << xs << endl;
		if (MyEqual(xm, xp)) break;
	}

	return xs;
}