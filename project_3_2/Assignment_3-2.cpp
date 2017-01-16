#include <iostream>
using namespace std;

void FindInt(int *x, int &n, int &find);

int main()
{
	cout << "Welcome to assignment 3, question 1!" << endl;
	int i, n, *x, findx;

	cout << "Give me the total number of numbers: ";
	cin >> n;

	//dynamically allocate memory
	x = new int[n];

	for (i = 0; i < n; i++) {
		cout << "Give me an integer: ";
		cin >> x[i];
	}

	cout << "Give me an integer to check: ";
	cin >> findx;

	FindInt(x, n, findx);

	//deallocate memory
	delete[] x;
	system("pause");
	return 0;
}

void FindInt(int *x, int &n, int &find)
{
	//linear search
	bool result = false;
	for (int i = 0; i < n; i++) {
		if (x[i] == find) {
			cout << "FOUND " << find << " at offset " << i << endl; result = true;
		}
	}
	if (!result) cout << "Integer " << find << " not found!" << endl;
}