#include<string>
#include<vector>
#include<fstream>
#include"Asset.h"
#include"BinomialTree.h"

BinomialTree::BinomialTree() //default size N = 1000
{
	n = 1000; Opt_Px.resize(n + 1); Eqt_Px.resize(n + 1); Ex_Bdy.resize(n + 1); opt = 0;
}
BinomialTree::BinomialTree(int N)
{
	n = N; Opt_Px.resize(n + 1); Eqt_Px.resize(n + 1); Ex_Bdy.resize(n + 1); opt = 0;
}
void BinomialTree::Resize(int N)
{
	n = N; Opt_Px.resize(n + 1); Eqt_Px.resize(n + 1); Ex_Bdy.resize(n + 1); opt = 0;
}
BinomialTree::~BinomialTree()
{
	opt = 0;
}
double BinomialTree::valueOption(Option *OPT)
{
	//setting parameters (q1 & q2 represent probabilities multiplied by discount factor)
	opt = OPT;

	double NowT = OPT->getTTM(), dT = NowT / n;
	double d = exp(-OPT->getSigma()*sqrt(dT)), u = 1.0 / d;

	double q1 = (1 - d*exp(-OPT->getRate()*dT)) / (u - d);
	double q2 = (u*exp(-OPT->getRate()*dT) - 1) / (u - d);

	//initialize value at maturity
	for (int i = 0; i <= n; i++)
	{
		Eqt_Px[i] = OPT->getSpot()*pow(u, n - i)*pow(d, i);
		Opt_Px[i] = OPT->getPayout(Eqt_Px[i], NowT);
		Ex_Bdy[i] = 0;
	}

	int i = 0;
	while (i < n) //check exercise at maturity
	{
		if (Opt_Px[i] == 0 && Opt_Px[i + 1] != 0) Ex_Bdy[n] = Eqt_Px[i + 1]; //put
		if (Opt_Px[i] != 0 && Opt_Px[i + 1] == 0) Ex_Bdy[n] = Eqt_Px[i]; //call
		i++;
	}

	for (int j = n - 1; j >= 0; j--)
	{
		NowT -= dT;
		bool this_ex = false, prev_ex;

		//calculate greeks
		if (j == 0) OPT->setDelta((Opt_Px[0] - Opt_Px[1]) / (Eqt_Px[0] - Eqt_Px[1]));
		if (j == 1)
		{
			OPT->setGamma((((Opt_Px[0] - Opt_Px[1]) / (Eqt_Px[0] - Eqt_Px[1])) - ((Opt_Px[1] - Opt_Px[2]) / (Eqt_Px[1] - Eqt_Px[2]))) 
				/ ((Eqt_Px[0] - Eqt_Px[2])*0.5));
		}

		for (int i = 0; i <= j; i++)
		{
			prev_ex = this_ex;
			Eqt_Px[i] /= u;
			Opt_Px[i] = (Opt_Px[i] * q1 + Opt_Px[i + 1] * q2);
			double EV = OPT->getPayout(Eqt_Px[i], NowT);
			
			if (EV > Opt_Px[i])
			{
				Opt_Px[i] = EV;
				this_ex = true;
			}
			else this_ex = false;

			if (this_ex && i == 0) Ex_Bdy[j] = Eqt_Px[0];
			if (this_ex && !prev_ex) Ex_Bdy[j] = Eqt_Px[i]; //max price for exercising put
			if (!this_ex && prev_ex) Ex_Bdy[j] = Eqt_Px[i - 1]; //min price for exercising call

		}
	}
	return Opt_Px[0];
}

void BinomialTree::ExerciseBoundary(char fileName[])
{
	ofstream outFile(fileName);
	string line;

	if (outFile)
	{
		double dT = opt->getTTM() / n;
		for (int i = 0; i <= n; i++) if (Ex_Bdy[i]) outFile << dT*i << "," << Ex_Bdy[i] << endl;
	}
	outFile.close();
}