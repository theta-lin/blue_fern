#ifndef LINEAR_HPP
#define LINEAR_HPP

#include <SFML/Graphics.hpp>
#include <cmath>

struct Vector2D
{
	sf::Vector2f p1;
	sf::Vector2f p2;
};

// ax + by + c = 0
struct Linear
{
	float a;
	float b;
	float c;
};

Linear toLinear(const Vector2D &vec);

// line-vector intersect
bool intersect(const Linear &line, const Vector2D &vec);
// vector-vector intersect
bool intersect(const Vector2D &vec1, const Vector2D &vec2);

const float pi{3.14159265358979323846f};

// get magnitude of a vector
float getMag(const Vector2D &vec);
// angle between vectors in radian
float angleVec(const Vector2D &vec1, const Vector2D &vec2);

//check if a point is inside a polygon
bool isPointInside(const sf::Vector2f &vec, const std::vector<sf::Vector2f> &polygon);
//check if a segment is inside or outside a polygon
bool isSegmentInside(const Vector2D &segment, const std::vector<sf::Vector2f> &polygon, const int maxIntersect);

sf::Vector2f move(const sf::Vector2f &origin, float dist, float angle);

const unsigned int prime{100003};

#endif
