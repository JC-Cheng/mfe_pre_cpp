//"This product includes software developed by the University of Chicago, as Operator of Argonne National Laboratory.
#include<iostream>
#include"Bond.h"
#include"TermStructure.h"
using namespace std;

int main() {

	int ans;
	cout << "Welcome to assignment 4: Bonds & Term Structures." << endl;
	cout << "Please enter 1: for 4-1(d)" << endl;
	cout << "             2: for 4-2(c)" << endl;
	cout << "             3: for 4-2(g)" << endl; 
	cout << "             0: to exit program : ";
	cin >> ans;

	while (ans)
	{
		if (ans == 1)
		{
			cout << endl << " ***** DEMO assignment 4-1(d) *****" << endl << endl;
			Bond Bd;
			Bd.user_defined_calculator();
			cout << endl << " **********************************" << endl << endl;
		}
		else if (ans == 2)
		{
			cout << endl << " ***** DEMO assignment 4-2(c) *****" << endl << endl;

			double T, t, C, P;
			cout << "Enter the time to maturity (in years) of bond when issued:";
			cin >> T;
			cout << "Enter the current time:";
			cin >> t;
			cout << "Enter coupon payment:";
			cin >> C;
			cout << "Enter principal payment:";
			cin >> P;
			Bond Bd(T, C, P, t);

			double b1, b2, b3, l2, l3;
			cout << "Now give me the Nelson Siegel parameters b1, b2, b3, l2, l3:" << endl;
			cin >> b1 >> b2 >> b3 >> l2 >> l3;
			TermStructure CRV(b1, b2, b3, l2, l3);
			
			cout << "The price of the bond with the given term structure is " << CRV.calculate_bondPx(&Bd) << endl << endl;
			cout << "The short rate is " << CRV.get_shortRate() << endl;
			cout << "The forward rate F(2,3) is " << CRV.get_fwdRate(2, 3) << endl;
			cout << "The 5 year zero coupon yield is " << CRV.get_yield(5) << endl;
			cout << endl << " **********************************" << endl << endl;
		}
		else if (ans == 3)
		{
			cout << endl << " ***** DEMO assignment 4-2(g) *****" << endl << endl;
			const int numbonds = 10;
			double P[numbonds] = { 100,100,100,100,100,100,100,100,100,1000 };
			double C[numbonds] = { 3,3,4,5,5,6,6,6,6,90 };
			double T[numbonds] = { 1,2,3,4,5,6,7,8,9,10 };
			double t[numbonds] = { 0,0,0,0,0,0,0,0,0,0 };
			double Price[numbonds] = { 100,100,101,101,102,102,100,100,100,1015 };
			
			Bond *myBonds = new Bond[numbonds];
			for (int i = 0; i < numbonds; i++)
			{
				myBonds[i].set_bondTerm(T[i], C[i], P[i], t[i]);
				myBonds[i].set_price(Price[i]);
			}

			TermStructure myCurve;
			myCurve.fit_YieldCurve(myBonds, numbonds);
			cout << "Nelson-Siegel approximation of yield curve for t=0...10 years" << endl;
			myCurve.output_curve(0, 0.5, 20);
			myCurve.output_parameters();

			delete[] myBonds;
			cout << endl << " **********************************" << endl << endl;
		}
		
		cout << endl;
		cout << "Please enter 1: for 4-1(d)" << endl;
		cout << "             2: for 4-2(c)" << endl;
		cout << "             3: for 4-2(g)" << endl;
		cout << "             0: to exit program : ";
		cin >> ans;
	}

	system("pause");
	return 0;
}