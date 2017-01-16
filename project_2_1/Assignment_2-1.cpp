// find the root of f(x)=x^2+1.5x−1
#include <iostream>
#include <iomanip>

using namespace std;
const int N = 10;
int main()
{
	cout << "Please give the starting point x_0 = " ;
	double x; //x_0 is the starting point
	cin >> x;
	cout << "The resulf of "<< N <<" times of interations:" << endl;
	
	for (int i = 0; i < N; i++) 
	{
		x =  (1 - x*x)*2/3; //produce next x_i by iteration
		cout << "x_" << setw(2) << (i +1) << " = " << setw(5) << x  << endl;
	}

	system("pause");
	return 0;
}