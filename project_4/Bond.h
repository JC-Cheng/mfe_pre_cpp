#ifndef __BOND_H__
#define __BOND_H__

class Bond {

	friend class TermStructure;

public:
	Bond();
	Bond(double T0, double C0, double P0, double t0);
	~Bond();

	void set_price(double Price0);
	void set_YTM(double YTM0);
	void set_bondTerm(double T0, double C0, double P0, double t0);
	double get_price();
	double get_YTM();

	void calculate_price(); //calculate and update Price, and give a bond report
	void calculate_YTM(); //calculate and update YTM, and give a bond report
	
	void set_CF(); //dynamically set CF by (T, C, P, t)
	void print_CF();
	int get_N();
	void user_defined_calculator(); //a method for assignment 4-1-d

	double T;
	double t;
	double C;
	double P;

private:
	double Price;
	double YTM;

	//attributes and method associated with bond cash flows
	double *CF; //cash flows
	double *Tau; //time from now
	int N;

	//general bond pricing function and 1st derivative
	double F_BondPx(double &y);
	double F1_BondPx(double &y);
};

#endif // !__BOND_H__

