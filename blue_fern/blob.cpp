#include "blob.hpp"
#include "perlin.hpp"

float Blob::getHeight(const float x)
{
	float xLen;
	get("xLen") >> xLen;
	float yLen;
	get("yLen") >> yLen;
	float dist{1.f - std::abs(x - xLen / 2.f) / (xLen / 2.f)};
	return sqrt(dist) * yLen / 2.f;
}

bool Blob::check()
{	
	if (!TriangleArray::check())
		return false;

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

void Blob::generate()
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

	Perlin perlin{perlinScale, 0, yLen / 2 * yScale};
	float width{xLen / dotCount};

	sf::Vector2f begin{xOrigin - xLen / 2, yOrigin};
	polygon[0].push_back(begin);
	for (float x{0}; x <= dotCount; ++x)
	{
		sf::Vector2f vertex{xOrigin - xLen / 2 + x * width, yOrigin - getHeight(x * width) - perlin.get(x * width)};
		polygon[0].push_back(vertex);
	}

	sf::Vector2f end{xOrigin + xLen / 2, yOrigin};
	polygon[0].push_back(end);
	for (float x{0}; x <= dotCount; ++x)
	{
		sf::Vector2f vertex{xOrigin + xLen / 2 - x * width, yOrigin + getHeight(xLen - x * width) + perlin.get(x * width + prime)};
		polygon[0].push_back(vertex);
	}
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
