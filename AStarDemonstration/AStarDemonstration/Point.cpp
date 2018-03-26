
#include "Point.h"
using namespace pf;

Point::Point()
{
	x = 0;
	y = 0;
}

Point::Point(int x, int y)
{
	this->x = x;
	this->y = y;
}

Point::Point(const Point &p)
{
	this->x = p.x;
	this->y = p.y;
}

Point& Point::operator=(const Point &rv)
{
	this->x = rv.x;
	this->y = rv.y;
	return *this;
}

Point& Point::operator+=(const Point &rv)
{
	this->x = this->x + rv.x;
	this->y = this->y + rv.y;
	return *this;
}

Point pf::operator+(Point lv, const Point &rv)
{
	return lv += rv;
}

bool pf::operator== (const Point &lv, const Point &rv)
{
	return (lv.x == rv.x && lv.y == rv.y);
}

bool pf::operator!= (const Point &lv, const Point &rv)
{
	return !(lv == rv);
}

std::ostream& pf::operator<<(std::ostream &out, const Point &p)
{
	return out << "(" << p.x << ", " << p.y << ")";
}
