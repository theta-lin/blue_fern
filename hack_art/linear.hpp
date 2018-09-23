#ifndef LINEAR_HPP
#define LINEAR_HPP

#include <SFML/Graphics.hpp>

struct Vector2D
{
	sf::Vector2f p1;
	sf::Vector2f p2;
};

//ax + by + c = 0
struct Linear
{
	float a;
	float b;
	float c;
};

Linear toLinear(const Vector2D &vec);

//line-vector intersect
bool intersect(const Linear &line, const Vector2D &vec);
//vector-vector intersect
bool intersect(const Vector2D &vec1, const Vector2D &vec2);

#endif
