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

#endif
