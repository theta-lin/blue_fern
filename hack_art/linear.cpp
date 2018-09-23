#include "linear.hpp"

Linear toLinear(const Vector2D &vec)
{
	// to standard form ax + by + c = 0
	// x(y2 - y1) + y(x1 - x2) + (x2y1 - x1y2) = 0
	Linear line;
	line.a = vec.p2.y - vec.p1.y;
	line.b = vec.p1.x - vec.p2.x;
	line.c = vec.p2.x * vec.p1.y - vec.p1.x * vec.p2.y;
	return line;
}

bool intersect(const Linear &line, const Vector2D &vec)
{
	// let ax1 + by1 + c1 = d1
	// let ax2 + by2 + c2 = d2
	float d1{line.a * vec.p1.x + line.b * vec.p1.y + line.c};
	float d2{line.a * vec.p2.x + line.b * vec.p2.y + line.c};

	// if d1 and d2 have different signs,
	// the two points of the vector is on different sides of the line
	if (d1 < 0 && d2 > 0) return true;
	if (d1 > 0 && d2 < 0) return true;

	return false;
}

bool intersect(const Vector2D &vec1, const Vector2D &vec2)
{
	if (intersect(toLinear(vec1), vec2) && intersect(toLinear(vec2), vec1))
		return true;
	else
		return false;
}
