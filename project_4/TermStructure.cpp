#include"Bond.h"
#include"TermStructure.h"
#include<iostream>
#include<cmath>
#include <iomanip>
using namespace std;

//the GET methods
double TermStructure::get_beta1() { return beta1; }
double TermStructure::get_beta2() { return beta2; }
double TermStructure::get_beta3() { return beta3; }
double TermStructure::get_lambda2() { return lambda2; }
double TermStructure::get_lambda3() { return lambda3; }

double TermStructure::get_yield(double T)
{
	double y=beta1, T2 = T*lambda2, T3 = T*lambda3;

	if (lambda2 && T) y += beta2*(1 - exp(-T2)) / T2;
	else y += beta2;
	
	if (lambda3 && beta3 && T) y += beta3*(((1 - exp(-T3)) / T3) - exp(-T3));
	return y;
}
double TermStructure::get_shortRate()
{
	return beta1 + beta2;
}
double TermStructure::get_fwdRate(double S, double T)
{
	return (T*this->get_yield(T) - S*this->get_yield(S)) / (T - S);
}

//constructor
TermStructure::TermStructure()
{
	beta1 = beta2 = beta3 = 0.01;
	lambda2 = lambda3 = 1;
	BList = 0; BNum = 0;
}
TermStructure::TermStructure(double B1, double B2, double B3, double L2, double L3)
{
	beta1 = B1; beta2 = B2; beta3 = B3;
	lambda2 = L2; lambda3 = L3;
	BList = 0; BNum = 0;
}
//destructor
TermStructure::~TermStructure()
{
	delete[] BList;
}

double TermStructure::calculate_bondPx(Bond *B)
{
	B->set_CF();
	double px = 0;
	for (int i = 0; i < B->N; i++)  px += B->CF[i] * exp(-B->Tau[i] * this->get_yield(B->Tau[i]));

	return px;
}

void TermStructure::output_curve(double T0, double deltaT, int N)
{
	cout << "======================" << endl;
	for (int i = 0; i <= N; i++) cout << setw(6) << T0+i*deltaT << "    " << this->get_yield(T0 + i*deltaT) << endl;
	cout << "======================" << endl;
}
void TermStructure::output_parameters()
{
	cout << "Beta1    = " << this->get_beta1() << endl;
	cout << "Beta2    = " << this->get_beta2() << endl;
	cout << "Beta3    = " << this->get_beta3() << endl;
	cout << "Lambda2  = " << this->get_lambda2() << endl;
	cout << "Lambda3  = " << this->get_lambda3() << endl;
}

void TermStructure::fcn(int m, int n, double *lambda, double *residual, int *iflag)
{
	//transform Nelson Siegel parameters from function parameters to class members
	beta1 = lambda[0];
	beta2 = lambda[1];
	beta3 = lambda[2];
	lambda2 = lambda[3];
	lambda3 = lambda[4];
	
	//generate the residuals
	for (int i = 0; i < BNum; i++)
	{
		double pHat = this->calculate_bondPx(&BList[i]); //pricing by CRV
		residual[i] = pHat - BList[i].get_price(); //mkt px
	}
}

void TermStructure::fit_YieldCurve(Bond *B_List, int B_Num)
{
	BList = B_List;
	BNum = B_Num;
	NLS myNLS(this, 5, B_Num);
	const double myGuess[5] = { 0.1,0.1,0.1,0.1,0.1 };
	myNLS.SOLVE(myGuess);
	BList = 0;
}