#ifndef __COORDINATE_H__
#define __COORDINATE_H__


class coordinate
{
public:
	coordinate(){}
	coordinate(double xin, double yin) { x = xin; y = yin; }
	double x;
	double y;
};

class closedcurve
{
public:
	//closedcurve();
	//~closedcurve();

	virtual double area() = 0;
	virtual double circumference() = 0;
	virtual coordinate place(double a) = 0;
};

class ellipse : public closedcurve
{

};
class circle : public ellipse
{

};
class rectangle : public closedcurve
{

};
class square : public rectangle
{

};


#endif // !__COORDINATE_H__

