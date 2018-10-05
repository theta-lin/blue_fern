#ifndef CLOUND_HPP
#define CLOUND_HPP

#include "triangle_array.hpp"

class Cloud final : public TriangleArray
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
	const float yScale;
	const float light;
	const sf::Uint8 alpha;

	sf::Color getColor(const std::vector<sf::Vector2f> &shape) final;
	float getHeight(const float x);
	bool check() final;

public:
	Cloud() : TriangleArray(),
		      colorOffset{30},
		      perlinScale{120.f},
		      dotCount{20.f},
		      yScale{0.6f},
		      light{1.f},
		      alpha{235}
	{
		set("perfectAngle", "0.8");
		set("influence1", "1");
		set("influence2", "5");
		set("maxIteration", "20");
		set("maxIntersect", "2");
		set("maxEdge", "30");
	}

	void generate() final;
	std::unique_ptr<Object> clone() final { return std::make_unique<Cloud>(*this); }
};

#endif
