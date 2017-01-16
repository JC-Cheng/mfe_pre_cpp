#include"Bond.h"
#include<iostream>
#include<cmath>
using namespace std;

const double precision = 1.0e-35;

void Bond::set_price(double Price0) { if (Price0 > 0) Price = Price0; }
void Bond::set_YTM(double YTM0) { if (YTM0 > 0) YTM = YTM0; }
void Bond::set_bondTerm(double T0, double C0, double P0, double t0) { T = T0; C = C0; P = P0; t = t0; }
double Bond::get_price() { return Price; }
double Bond::get_YTM() { return YTM; }
int Bond::get_N() { return N; }

Bond::Bond() //default setting
{
	T = 5; t = 0; C = 1; P = 100; Price = 100; YTM = 0.01; N = 0; CF = 0; Tau = 0;
}
Bond::Bond(double T0, double C0, double P0, double t0) //customer setting
{
	T = T0; t = t0; C = C0; P = P0; 
	Price = 100; YTM = 0.01;
	N = 0; CF = 0; Tau = 0;
}
Bond::~Bond() //destructor
{
	delete[] CF, Tau;
}

void Bond::calculate_price()
{
	this->set_CF();
	Price = this->F_BondPx(YTM);
	cout << "The bond (T = " << T << ", C = " << C << ", P = " << P << ")" << endl;
	cout << "at current time t = " << t << ", is priced at " << Price << ", using YTM = " << YTM * 100 << "%" << endl << endl;
}

void Bond::calculate_YTM()
{
	this->set_CF();
	double f, x_next, x_prev = 0.05; //using 3% as the initial guess for YTM
	f = this->F_BondPx(x_prev) - Price;
	while (abs(f) > precision) {
		x_next = x_prev - f / this->F1_BondPx(x_prev);
		x_prev = x_next;
		f = this->F_BondPx(x_prev) - Price;
	}
	YTM = x_prev;
	cout << "The bond (T = " << T << ", C = " << C << ", P = " << P << ")" << endl;
	cout << "at current time t = " << t << ", is priced at YTM = " << YTM*100 << "%, given price of " << Price << endl << endl;
}
void Bond::user_defined_calculator()
{

	cout << "Enter the time to maturity (in years) of bond when issued:";
	cin >> T;
	cout << "Enter the current time:";
	cin >> t;
	cout << "Enter coupon payment:";
	cin >> C;
	cout << "Enter principal payment:";
	cin >> P;
	
	cout << "Enter (continuous compounded) YTM:";
	cin >> YTM;
	cout << endl;
	this->calculate_price();

	cout << "Now give me another price:";
	cin >> Price;
	cout << endl;
	this->calculate_YTM();
}
void Bond::set_CF()
{
	delete[] CF, Tau;
	if (C == 0) //zero coupon bond
	{
		N = 1;
		CF = new double[N]; Tau = new double[N];
		CF[0] = P; Tau[0] = T - t;
	}
	else if (T == floor(T)) //integer TTM; standard coupon schedule
	{
		N = T - floor(t);
		CF = new double[N]; Tau = new double[N];
		for (int i = 0; i < N; i++)
		{
			CF[i] = C; Tau[i] = i + 1 + floor(t) - t;
		}
		CF[N - 1] += P;
	}
	else //arbitrary TTM; if TTM = z + r where z is an interger and 0 < r < 1, then 1st coupon pay at time 1 + r
	{
		double r = T - floor(T);
		if (t < 1 + r) //1st coupon hasn't made yet
		{
			N = floor(T);
			CF = new double[N]; Tau = new double[N];
			for (int i = 0; i < N; i++)
			{
				CF[i] = C; Tau[i] = i + 1 + r - t;
			}
			CF[0] += C*r;
			CF[N - 1] += P;
		}
		else //1st coupon made
		{
			N = ceil(T - t);
			CF = new double[N]; Tau = new double[N];
			for (int i = 0; i < N; i++)
			{
				CF[i] = C; Tau[i] = T - (N - i - 1) - t;
			}
			CF[N - 1] += P;
		}
	}
}

void Bond::print_CF()
{
	this->set_CF();
	cout << "=== This bond has " << N << " CFs ===" << endl;
	for (int i = 0; i < N; i++) {
		cout << "CF_" << i + 1 << " : " << CF[i] << " at time " << Tau[i] << endl;
	}
	cout << endl;
}

double Bond::F_BondPx(double &y) //general bond pricing, a function of YTM y
{
	double px = 0;
	for (int i = 0; i < N; i++)
	{
		px += CF[i] * exp(-Tau[i] * y);
	}
	return px;
}
double Bond::F1_BondPx(double &y) //first derivaitve of the bond pricing function
{
	double f1 = 0;
	for (int i = 0; i < N; i++)
	{
		f1 += -CF[i] * Tau[i] * exp(-Tau[i] * y);
	}
	return f1;
}