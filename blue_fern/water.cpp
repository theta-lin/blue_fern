#include "water.hpp"
#include "perlin.hpp"

sf::Color Water::getColor(const std::vector<sf::Vector2f> &shape)
{
	static time_t master{time(0)};

	int gTotal{static_cast<int>(3 * 255 * green)};
	int bTotal{static_cast<int>(3 * 255 * blue)};
	std::uniform_int_distribution<int> dist{-colorOffset, colorOffset};
	for (auto &point : shape)
	{
		std::mt19937 generator{static_cast<unsigned int>(master) * (static_cast<unsigned int>(point.x) + static_cast<unsigned int>(point.y) * prime)};
		int randOffset{dist(generator)};
		gTotal += randOffset;
		bTotal += randOffset;
	}

	gTotal = std::min(gTotal, 3 * 255);
	gTotal = std::max(gTotal, 0);
	bTotal = std::min(bTotal, 3 * 255);
	bTotal = std::max(bTotal, 0);

	return {
				0,
				static_cast<sf::Uint8>(gTotal / 3),
				static_cast<sf::Uint8>(bTotal / 3),
				alpha
	       };
}

bool Water::check()
{
	if (!db.count("xOrigin"))
	{
		std::cerr << "variable \"xOrigin\" not set" << std::endl;
		return false;
	}
	if (!db.count("yOrigin"))
	{
		std::cerr << "variable \"yOrigin\" not set" << std::endl;
		return false;
	}
	if (!db.count("xLen"))
	{
		std::cerr << "variable \"xLen\" not set" << std::endl;
		return false;
	}
	if (!db.count("yLen"))
	{
		std::cerr << "variable \"yLen\" not set" << std::endl;
		return false;
	}

	return true;
}

void Water::generate()
{
	std::lock_guard<std::mutex> guard{mutex};
	if (!check())
		return;

	clear();

	float xOrigin;
	get("xOrigin") >> xOrigin;
	float yOrigin;
	get("yOrigin") >> yOrigin;
	float xLen;
	get("xLen") >> xLen;
	float yLen;
	get("yLen") >> yLen;

	Perlin perlin{perlinScale, yLen * yScaleMin, yLen * yScaleMax};
	float width{xLen / dotCount};

	sf::Vector2f begin{xOrigin - xLen / 2, yOrigin};
	polygon[0].push_back(begin);
	for (float x{0}; x <= dotCount; ++x)
	{
		sf::Vector2f vertex{xOrigin - xLen / 2 + x * width, yOrigin - perlin.get(x * width)};
		polygon[0].push_back(vertex);
	}
	sf::Vector2f end{xOrigin + xLen / 2, yOrigin};
	polygon[0].push_back(end);
	polygon[0].push_back(polygon[0][0]);

	dividePolygon();
	divideTriangles();

	for (auto &triangle : polygon)
	{
		sf::Color color{getColor(triangle)};
		for (auto &vertex : triangle)
			vertices.push_back({vertex, color});
	}

	canDraw = true;
}
