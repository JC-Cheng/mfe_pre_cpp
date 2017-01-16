#ifndef __ASSET_H__
#define __ASSET_H__
#include<string>
//BASE class asset
class Asset
{
public:
	virtual double getValue() = 0;
};

//Stock
class Stock :public Asset
{
public:
	Stock();
	Stock(double Spot, double Sigma);
	double getValue();
	double getVol();
	void setValue(double v);
	void setVol(double v);
protected:
	double spot, sigma;
};

//Base option class
class Option :public Asset 
{
public:
	virtual double BlackScholesValue() = 0;
	
	Option(double Spot, double Strike, double Sigma, double rate, double TTM);
	~Option();

	virtual char const* getOptionType() = 0;
	virtual double getPayout(double s, double t) = 0;

	double getStrike();
	double getSpot();
	double getSigma();
	double getRate();
	double getTTM();
	double getDelta();
	double getGamma();

	void setDelta(double d);
	void setGamma(double g);

protected:
	double K, T, rc;
	double delta, gamma;
	Stock *S;
};

//Call Options
class Call :public Option
{
public:
	Call(double Spot, double Strike, double Sigma, double rate, double TTM);
	double BlackScholesValue();
};
class AmericanCall :public Call
{
public:
	AmericanCall(double Spot, double Strike, double Sigma, double rate, double TTM);
	double getValue();
	char const* getOptionType();
	double getPayout(double s, double t);
};
class EuropeanCall :public Call
{
public:
	EuropeanCall(double Spot, double Strike, double Sigma, double rate, double TTM);
	double getValue();
	char const* getOptionType();
	double getPayout(double s, double t);
};

//Put Options
class Put :public Option
{
public:
	Put(double Spot, double Strike, double Sigma, double rate, double TTM);
	double BlackScholesValue();
};
class AmericanPut :public Put
{
public:
	AmericanPut(double Spot, double Strike, double Sigma, double rate, double TTM);
	double getValue();
	char const* getOptionType();
	double getPayout(double s, double t);
};
class EuropeanPut :public Put
{
public:
	EuropeanPut(double Spot, double Strike, double Sigma, double rate, double TTM);
	double getValue();
	char const* getOptionType();
	double getPayout(double s, double t);
};

#endif

