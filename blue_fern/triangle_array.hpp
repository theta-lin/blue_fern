#ifndef TRIANGLE_ARRAY_HPP
#define TRIANGLE_ARRAY_HPP

#include "object.hpp"
#include "linear.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <algorithm>
#include <vector>
#include <random>
#include <limits>

class TriangleArray : public Object
{
	/*
	 * Require:
	 * perfectAngle (in number of pi's)
	 * influence1
	 * influence2
	 * maxIteration
	 * maxIntersect
	 * maxEdge
	 */

protected:
	const float maxFloat{std::numeric_limits<float>::max()};
	std::vector<std::vector<sf::Vector2f> > polygon;
	std::vector<sf::Vertex> vertices;
	
	float getScore(const Vector2D e1, const Vector2D e2, const Vector2D e3, const Vector2D e4);
	void dividePolygon();
	void divideTriangles();
	virtual sf::Color getColor(const std::vector<sf::Vector2f> &shape) = 0;

	bool check() override;

public:
	TriangleArray() : Object()
	{
		polygon.resize(1);
	}
	void draw(sf::RenderTarget &target) final;
};

#endif
