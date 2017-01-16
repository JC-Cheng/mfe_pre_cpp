//#pragma once
#ifndef __DataSeries__
#define __DataSeries__

class DataSeries
{
public:
	DataSeries(int k);
	~DataSeries();

	double getValue(int k);
	void setValue(double x, int k);
	double AbsoluteVariation();
	
	//this naive method take O(N^3) of time complexity
	int CountChangeAboveThreshold_N3(double T);

	//this one is the improved method to find the correct Q
	int CountChangeAboveThreshold(double T);

	//TA suggested
	int CountChangeAboveThreshold_NLgN(double T);

private:
	double *data; //declare data series using dynamic memory allocation
	int dLen; //the length of data
};
#endif