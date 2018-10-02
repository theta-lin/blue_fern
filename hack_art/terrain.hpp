#ifndef TERRAIN_HPP
#define TERRAIN_HPP

#include "triangle_array.hpp"
#include "perlin.hpp"

class Terrain : public TriangleArray
{

	/*
	 * Require:
	 * temp
	 * humid
	 * minHeight
	 * maxHeight
	 * screenWidth
	 * screenHeight
	 * dotCount
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

	sf::Color colorMix(const sf::Color &a, const sf::Color &b, const float ratio);
	sf::Color colorTempHumid(float temp, float humid);
	sf::Color getColor(const sf::ConvexShape &shape);

	bool check() override;

public:
	Terrain() : TriangleArray(),
	            hotDry   {255, 0,   0  },
		        warmDry  {255, 255, 0  },
		        coldDry  {180, 220, 255},
		        hotHumid {0,   80 , 0  },
	        	warmHumid{0,   255, 0  },
		        coldHumid{100, 255, 255},
				heightDelta{-0.002f},
		        colorOffset{50},
		        perlinScale{150.f}
	{
		set("perfectAngle", "0.8");
		set("influence1", "1");
		set("influence2", "5");
		set("maxIteration", "20");
		set("maxIntersect", "2");
		set("maxEdge", "50");
	}
	void generate() override;
};

#endif