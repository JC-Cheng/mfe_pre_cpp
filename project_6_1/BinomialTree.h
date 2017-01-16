#ifndef __BINOMINALTREE_H__
#define __BINOMINALTREE_H__
#include<string>
#include<vector>
#include"Asset.h"

using namespace std;

class BinomialTree
{
public:
	BinomialTree();
	BinomialTree(int N);
	~BinomialTree();
	void Resize(int N);
	double valueOption(Option *OPT);
	void ExerciseBoundary(char fileName[]); //output a series of (t,s) for exercise boundary

private:
	int n;
	vector<double> Opt_Px, Eqt_Px, Ex_Bdy;
	Option* opt;
};


#endif