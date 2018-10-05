#ifndef BLOB_HPP
#define BLOB_HPP

#include "triangle_array.hpp"

class Blob : public TriangleArray
{

	/*
	 * Require:
	 * xOrigin
	 * yOrigin
	 * xLen
	 * yLen
	 */

private:
	const float perlinScale;
	const float dotCount;
	const float yScale;

	sf::Color getColor(const std::vector<sf::Vector2f> &shape) = 0;
	float getHeight(const float x);
	bool check() final;

protected:
	const int colorOffset;

public:
	Blob() : TriangleArray(),
		     colorOffset{30},
		     perlinScale{120.f},
	     	 dotCount{20.f},
		     yScale{0.6f}
	{
		set("perfectAngle", "0.8");
		set("influence1", "1");
		set("influence2", "5");
		set("maxIteration", "20");
		set("maxIntersect", "2");
		set("maxEdge", "30");
	}

	void generate() final;
};

#endif
