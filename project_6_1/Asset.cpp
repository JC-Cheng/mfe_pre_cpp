#include"Asset.h"
#include<cmath>
#include<iostream>
#include<string>
#include"BinomialTree.h"
using namespace std;
double N_cdf(const double x);

//methods asscociated with STOCK class
Stock::Stock() { spot = 0; sigma = 0; }
Stock::Stock(double value, double volatility) { spot = value; sigma = volatility; }
double Stock::getValue() { return spot; }
double Stock::getVol() { return sigma; }
void Stock::setValue(double v) { spot = v; }
void Stock::setVol(double v) { sigma = v; }

Option::Option(double Spot, double Strike, double Sigma, double rate, double TTM)
{
	S = new Stock(Spot, Sigma);
	K = Strike; T = TTM; rc = rate;
	delta = -99999; gamma = -99999; //a default value indicating greeks haven't been calculated yet
}
Option::~Option()
{
	delete S;
}
double Option::getStrike() { return K; }
double Option::getSpot() { return S->getValue(); }
double Option::getSigma() { return S->getVol(); }
double Option::getRate() { return rc; }
double Option::getTTM() { return T; }
double Option::getDelta()
{
	if (delta == -99999) cout << "Invalid access of DELTA, please price the option by binomial model first!" << endl;
	return delta;
}
double Option::getGamma()
{
	if (gamma == -99999) cout << "Invalid access of GAMMA, please price the option by binomial model first!" << endl;
	return gamma;
}

void Option::setDelta(double d) { delta = d; }
void Option::setGamma(double g) { gamma = g; }

Call::Call(double Spot, double Strike, double Sigma, double rate, double TTM) :
	Option(Spot, Strike, Sigma, rate, TTM) {}
AmericanCall::AmericanCall(double Spot, double Strike, double Sigma, double rate, double TTM) :
	Call(Spot, Strike, Sigma, rate, TTM) {}
EuropeanCall::EuropeanCall(double Spot, double Strike, double Sigma, double rate, double TTM) :
	Call(Spot, Strike, Sigma, rate, TTM) {}

Put::Put(double Spot, double Strike, double Sigma, double rate, double TTM) :
	Option(Spot, Strike, Sigma, rate, TTM) {}
AmericanPut::AmericanPut(double Spot, double Strike, double Sigma, double rate, double TTM) :
	Put(Spot, Strike, Sigma, rate, TTM) {}
EuropeanPut::EuropeanPut(double Spot, double Strike, double Sigma, double rate, double TTM) :
	Put(Spot, Strike, Sigma, rate, TTM) {}

//return 2-char ticker
char const* AmericanCall::getOptionType() { return "AC"; }
char const* EuropeanCall::getOptionType() { return "EC"; }
char const* AmericanPut::getOptionType() { return "AP"; }
char const* EuropeanPut::getOptionType() { return "EP"; }

//return exercise value
double AmericanCall::getPayout(double s, double t) { return (s > K ? s - K : 0); }
double EuropeanCall::getPayout(double s, double t) { return (t == T ? (s > K ? s - K : 0) : 0); }
double AmericanPut::getPayout(double s, double t) { return (s > K ? 0 : K - s); }
double EuropeanPut::getPayout(double s, double t) { return (t == T ? (s > K ? 0 : K - s) : 0); }

double AmericanCall::getValue()
{
	BinomialTree BTree;
	return BTree.valueOption(this);
	//return 0;
}

double AmericanPut::getValue()
{
	BinomialTree BTree;
	return BTree.valueOption(this);
	//return 0;
}

double EuropeanCall::getValue() { return this->BlackScholesValue(); }
double EuropeanPut::getValue() { return this->BlackScholesValue(); }

double Call::BlackScholesValue()
{
	double x = (log(S->getValue() / K) + (rc + S->getVol()*S->getVol()*0.5)*T) / (S->getVol()*sqrt(T));
	return S->getValue()*N_cdf(x) - K*exp(-rc*T)*N_cdf(x - S->getVol()*sqrt(T));
}
double Put::BlackScholesValue()
{
	double x = (log(S->getValue() / K) + (rc + S->getVol()*S->getVol()*0.5)*T) / (S->getVol()*sqrt(T));
	return K*exp(-rc*T)*(1 - N_cdf(x - S->getVol()*sqrt(T))) - S->getValue()*(1 - N_cdf(x));
}

//Helping functions
double N_cdf(const double x)
{
	const double b1 = 0.319381530;
	const double b2 = -0.356563782;
	const double b3 = 1.781477937;
	const double b4 = -1.821255978;
	const double b5 = 1.330274429;
	const double p = 0.2316419;
	const double c = 0.39894228;

	if (x >= 0.0)
	{
		double t = 1.0 / (1.0 + p*x);
		return (1.0 - c*exp(-x*x / 2.0)*t*(t*(t*(t*(t*b5 + b4) + b3) + b2) + b1));
	}
	else
	{
		double t = 1.0 / (1.0 - p*x);
		return (c*exp(-x*x / 2.0)*t*(t*(t*(t*(t*b5 + b4) + b3) + b2) + b1));
	}
}
