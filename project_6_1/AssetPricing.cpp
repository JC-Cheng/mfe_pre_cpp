#include<iostream>
#include<fstream>
#include<string>
#include"Asset.h"
#include"BinomialTree.h"
using namespace std;

//part 1-e
void ValueSpot(double lowerSpotPrice, double upperSpotPrice, int numberOfSpots, char CallFileName[], char PutFileName[]);
//part 1-f
void ValueOption(char inName[], char outName[]);
//part 2-b (output file is not required)
void FindTreeDepth();
//part 2-c
void CompareEPAP(double lowerStrike, double upperStrike, int numberOfStrike, char FileName[]);
//part 2-d
void ValueDelta(char inName[],char outName[]);

bool nextToken(const string& line, int& offset, string& token);
bool parseLine(const string& line, Option* &opt);

//MAIN FUNCTION
int main()
{
	
	ValueSpot(75, 125, 51, "SpotCALL_Part 1-e.CSV", "SpotPUT_Part 1-e.CSV");
	ValueOption("ValueOptionsInput.CSV", "ValueOptionOutput 1-f.CSV");
	FindTreeDepth();
	CompareEPAP(80, 110, 31, "Compare_Part 2-c.CSV");
	ValueDelta("ValueDeltaInput.CSV", "ValueDeltaOutput 2-d.CSV"); //output delta and gamma

	//part 2-e
	Option *O = new AmericanPut(90, 100, 0.20, 0.05, 1);
	BinomialTree Tree(1000);
	
	Tree.valueOption(O);
	Tree.ExerciseBoundary("Exercise Boundary SIGMA 20.CSV");
	
	delete O;
	O = new AmericanPut(90, 100, 0.40, 0.05, 1);
	
	Tree.valueOption(O);
	Tree.ExerciseBoundary("Exercise Boundary SIGMA 40.CSV");

	system("pause");
	return 0;
}

//a method to output BS-Value for European calls and puts (for assignment 6-1 Part 1, e)
void ValueSpot(double lowerSpotPrice, double upperSpotPrice, int numberOfSpots, char CallFileName[], char PutFileName[])
{
	double K = 100, RF = 0.05, Vol = 0.40, T = 1.0;
	ofstream CF(CallFileName),PF(PutFileName);
	if (CF && PF)
	{
		double dp = (upperSpotPrice - lowerSpotPrice) / (numberOfSpots - 1);
		for (int i = 0; i <= numberOfSpots; i++)
		{
			EuropeanCall EC(lowerSpotPrice + i*dp, K, Vol, RF, T);
			EuropeanPut EP(lowerSpotPrice + i*dp, K, Vol, RF, T);
			CF << "EC" << "," << K << "," << lowerSpotPrice + i*dp << "," << Vol << "," << T << "," << RF << "," << EC.getValue() << endl;
			PF << "EP" << "," << K << "," << lowerSpotPrice + i*dp << "," << Vol << "," << T << "," << RF << "," << EP.getValue() << endl;		
		}
		CF.close();
		PF.close();
	}
}

//a method to read options input and write values using BS formula (for assignment 6-1 Part 1, f)
void ValueOption(char inName[], char outName[])
{
	ifstream inFile(inName); ofstream outFile(outName);
	string line;
	Option *opt = 0;

	if (inFile && outFile)
	{
		while (getline(inFile, line))
		{
			if (parseLine(line, opt))
			{
				outFile << opt->getOptionType() << "," << opt->getStrike() << "," << opt->getSpot() << "," 
					<< opt->getSigma() << "," << opt->getTTM() << "," << opt->getRate() << "," << opt->getValue() << endl;
				delete opt;
			}
			else cout << "error reading line: '" << line << "'" << endl;
		}
		outFile.close();
		inFile.close();
	}
}

void FindTreeDepth()
{
	Option *C = new EuropeanCall(90, 90, 0.25, 0.08, 0.5);
	double BS_Value = C->getValue(), BT_Value = 0;
	int k = 100;
	do
	{
		BinomialTree OptPricer(k);
		BT_Value = OptPricer.valueOption(C);
		k++;

	} while (abs(BT_Value - BS_Value) > 0.01);
	cout << "N = " << k - 1 << ", Binomial Model Value = " << BT_Value << endl;
	cout << "BlackScholes Model Value = " << BS_Value << endl;
}

//a method to read options input and write values using BS formula (for assignment 6-1 Part 2, c)
void CompareEPAP(double lowerStrike, double upperStrike, int numberOfStrike, char FileName[])
{
	double S = 100, RF = 0.06, Vol = 0.30, T = 2.0;
	ofstream oF(FileName);
	if (oF)
	{
		double dp = (upperStrike - lowerStrike) / (numberOfStrike - 1);
		for (int i = 0; i < numberOfStrike; i++)
		{
			EuropeanPut EP(S, lowerStrike + i*dp, Vol, RF, T);
			AmericanPut AP(S, lowerStrike + i*dp, Vol, RF, T);
			oF << S << "," << lowerStrike + i*dp << "," << Vol << "," << T << "," << RF << "," << EP.getValue() << "," << AP.getValue() << endl;
			
		}
		oF.close();
	}



}

//a method to read options input and write values, delta and gamma using binomial model (for assignment 6-1 Part 2, d)
void ValueDelta(char inName[], char outName[])
{
	ifstream inFile(inName); ofstream outFile(outName);
	string line;
	Option *opt = 0;
	BinomialTree Tree(1000);

	if (inFile && outFile)
	{
		while (getline(inFile, line))
		{
			if (parseLine(line, opt))
			{
				double OptionValue = Tree.valueOption(opt);
				outFile << opt->getOptionType() << "," << opt->getStrike() << "," << opt->getSpot() << ","
					<< opt->getSigma() << "," << opt->getTTM() << "," << opt->getRate() << "," 
					<< OptionValue << "," << opt->getDelta() << "," << opt->getGamma() << endl;
				delete opt;
			}
			else cout << "error reading line: '" << line << "'" << endl;
		}
		outFile.close();
		inFile.close();
	}
}

//helping functions for loading inputs
bool nextToken(const string& line, int& offset, string& token)
{
	int len = line.length();
	bool rc = (offset < len);
	token = "";
	bool done = false;
	while (!done && (offset < len))
	{
		char c = line.at(offset);
		if (c == ',' || c == '\r' || c == '\n' || c == '\t' || c == ' ') done = true;
		else token += c;
		offset++;
	}
	return rc;
}
bool parseLine(const string& line, Option* &opt)
{
	int offset = 0;
	string Opt_Type,Strike,Spot,Sigma,T,r;

	bool rc = nextToken(line, offset, Opt_Type);
	rc &= nextToken(line, offset, Strike);
	rc &= nextToken(line, offset, Spot);
	rc &= nextToken(line, offset, Sigma);
	rc &= nextToken(line, offset, T);
	rc &= nextToken(line, offset, r);

	if (Opt_Type == "EC")
	{
		opt = new EuropeanCall(atof(Spot.c_str()), atof(Strike.c_str()), atof(Sigma.c_str()), atof(r.c_str()), atof(T.c_str()));
	}
	else if (Opt_Type == "EP")
	{
		opt = new EuropeanPut(atof(Spot.c_str()), atof(Strike.c_str()), atof(Sigma.c_str()), atof(r.c_str()), atof(T.c_str()));
	}
	else if (Opt_Type == "AC")
	{
		opt = new AmericanCall(atof(Spot.c_str()), atof(Strike.c_str()), atof(Sigma.c_str()), atof(r.c_str()), atof(T.c_str()));
	}
	else if (Opt_Type == "AP")
	{
		opt = new AmericanPut(atof(Spot.c_str()), atof(Strike.c_str()), atof(Sigma.c_str()), atof(r.c_str()), atof(T.c_str()));
	}
	return rc;
}