#ifndef TERRAIN_HPP
#define TERRAIN_HPP

#include "triangle_array.hpp"
#include "perlin.hpp"

class Terrain final : public TriangleArray
{

	/*
	 * Require:
	 * temp
	 * humid
	 * minHeight
	 * maxHeight
	 * screenWidth
	 * screenHeight
	 */

private:
	const sf::Color hotDry;
	const sf::Color warmDry;
	const sf::Color coldDry;
	const sf::Color hotHumid;
	const sf::Color warmHumid;
	const sf::Color coldHumid;
	const float heightDelta;
	const int colorOffset;
	const float perlinScale;
	const float dotCount;

	sf::Color colorMix(const sf::Color &a, const sf::Color &b, const float ratio);
	sf::Color colorTempHumid(float temp, float humid);
	sf::Color getColor(const std::vector<sf::Vector2f> &shape) final;

	bool check() final;

public:
	Terrain() : TriangleArray(),
	            hotDry   {255, 0,   0  },
		        warmDry  {255, 255, 0  },
		        coldDry  {180, 220, 255},
		        hotHumid {0,   80 , 0  },
	        	warmHumid{0,   220, 0  },
		        coldHumid{150, 250, 255},
				heightDelta{-0.0015f},
		        colorOffset{50},
		        perlinScale{150.f},
		        dotCount{50.f}
	{
		set("perfectAngle", "0.8");
		set("influence1", "1");
		set("influence2", "5");
		set("maxIteration", "20");
		set("maxIntersect", "2");
		set("maxEdge", "50");
	}

	void update() final;
	void generate() final;
	std::unique_ptr<Object> clone() final { return std::make_unique<Terrain>(*this); }
};

#endif
