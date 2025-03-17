#pragma once
class Point
{
	int x, y;
public:
	
	Point(int x, int y) : x(x), y(y) {}//constructor
	Point() : x(0), y(0) {}//defuault constructor
	Point(const Point& p) : x(p.x), y(p.y) {}//copy constructor
	Point& operator=(const Point& p) {
		x = p.x;
		y = p.y;
		return *this;
	}
	bool operator==(const Point& p) const {
		return x == p.x && y == p.y;
	}
	bool operator!=(const Point& p) const {
		return !(*this == p);
	}
	Point operator+(const Point& p) const {
		return Point(x + p.x, y + p.y);
	}
	Point operator-(const Point& p) const {
		return Point(x - p.x, y - p.y);
	}
	Point& operator+=(const Point& p) {
		x += p.x;
		y += p.y;
		return *this;
	}
	Point& operator-=(const Point& p) {
		x -= p.x;
		y -= p.y;
		return *this;
	}
	int getX() const { return x; }
	int getY() const { return y; }
	void setX(int x) { this->x = x; }
	void setY(int y) { this->y = y; }
};

