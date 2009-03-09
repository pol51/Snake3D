#ifndef __POINTS_H
#define __POINTS_H

using namespace std;

class Point
{
protected:
	int X;
	int Y;
public:
	inline Point() {X = 0; Y = 0;}
	inline Point(const int x, const int y) {X = x; Y = y;}
	inline Point operator+(const Point &pt) {return Point(X + pt.X, Y + pt.Y);}
	inline Point operator-(const Point &pt) {return Point(X - pt.X, Y - pt.Y);}
	inline int operator==(const Point &pt) const
    {return ((X == pt.X) and (Y == pt.Y));}
	inline int x() const {return X;}
	inline int y() const {return Y;}
	inline float xf(float fact) const {return float(X) / fact;} 
	inline float yf(float fact) const {return float(Y) / fact;}
	inline void x(int x) {X = x;}
	inline void y(int y) {Y = y;}
};

#endif
