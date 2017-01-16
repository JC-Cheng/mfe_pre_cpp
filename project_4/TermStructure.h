#ifndef __TERMSTRUCTURE_H__
#define __TERMSTRUCTURE_H__

#include"NLS.h"

class TermStructure : public FitCalculator
{
public:

	TermStructure();
	TermStructure(double B1, double B2, double B3, double L2, double L3);
	~TermStructure();

	double get_beta1();
	double get_beta2();
	double get_beta3();
	double get_lambda2();
	double get_lambda3();

	double get_yield(double T);
	double get_shortRate();
	double get_fwdRate(double S, double T);

	double calculate_bondPx(Bond *B);

	void output_curve(double T0, double deltaT, int N);
	void output_parameters();

	//our FitCalculator fcn method, used by NLS
	void fcn(int m, int n, double *lambda, double *residual, int *iflag);

	//read bonds and then fit a yield curve using N-S model
	void fit_YieldCurve(Bond *B_List, int B_Num);

private:
	double beta1, beta2, beta3;
	double lambda2, lambda3;
	Bond *BList;
	int BNum;
};
#endif
