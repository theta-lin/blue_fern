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

float getMag(const Vector2D &vec)
{
	return std::hypot(vec.p2.x - vec.p1.x, vec.p2.y - vec.p1.y);
}

float angleVec(const Vector2D &vec1, const Vector2D &vec2)
{
	float dx1{vec1.p2.x - vec1.p1.x};
	float dy1{vec1.p2.y - vec1.p1.y};
	float dx2{vec2.p2.x - vec2.p1.x};
	float dy2{vec2.p2.y - vec2.p1.y};
	float dotProduct{dx1 * dx2 + dy1 * dy2};

	return std::acos(dotProduct / (getMag(vec1) * getMag(vec2)));
}

bool isPointInside(const sf::Vector2f &vec, const std::vector<sf::Vector2f> &polygon)
{
	Vector2D segment{sf::Vector2f(-1.f, -1.f), vec};

	int intCount(0);
	for (size_t i{1}; i < polygon.size(); ++i)
	{
		Vector2D edge{polygon[i - 1], polygon[i]};
		if (intersect(segment, edge))
			++intCount;
	}

	Vector2D bottom{polygon[0], polygon[polygon.size() - 1]};
	if (intersect(segment, bottom))
		++intCount;

	if (intCount % 2 == 1)
		return true;
	else
		return false;
}

bool isSegmentInside(const Vector2D &segment, const std::vector<sf::Vector2f> &polygon, const int maxIntersect)
{
	int intCount{0};
	for (size_t i{1}; i < polygon.size(); ++i)
	{
		Vector2D edge{polygon[i - 1], polygon[i]};
		if (intersect(segment, edge))
			++intCount;
	}

	Vector2D bottom{polygon[0], polygon[polygon.size() - 1]};
	if (intersect(segment, bottom))
		++intCount;

	return intCount <= maxIntersect;
}

sf::Vector2f move(const sf::Vector2f &origin, float dist, float angle)
{
	sf::Vector2f pos;
	pos.x = origin.x + dist * std::cos(angle);
	pos.y = origin.y + dist * std::sin(angle);
	return pos;
}
