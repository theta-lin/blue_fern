#ifndef WATER_HPP
#define WATER_HPP

#include "triangle_array.hpp"

class Water final : public TriangleArray
{

	/*
	 * Require:
	 * xOrigin
	 * yOrigin
	 * xLen
	 * yLen
	 */

private:
	const int colorOffset;
	const float perlinScale;
	const float dotCount;
	const float yScaleMin;
	const float yScaleMax;
	const float green;
	const float blue;
	const sf::Uint8 alpha;

	sf::Color getColor(const std::vector<sf::Vector2f> &shape) final;
	bool check() final;

public:
	Water() : TriangleArray(),
		colorOffset{30},
		perlinScale{120.f},
		dotCount{30.f},
		yScaleMin{0.7f},
		yScaleMax{1.3f},
		green{0.3f},
		blue{1.f},
		alpha{220}
	{
		set("perfectAngle", "0.8");
		set("influence1", "1");
		set("influence2", "5");
		set("maxIteration", "20");
		set("maxIntersect", "2");
		set("maxEdge", "30");
	}

	void generate() final;
	std::unique_ptr<Object> clone() final { return std::make_unique<Water>(*this); }
};

#endif
