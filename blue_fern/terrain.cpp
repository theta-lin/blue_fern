#include "terrain.hpp"

sf::Color Terrain::colorMix(const sf::Color &a, const sf::Color &b, const float ratio)
{
	return {
				static_cast<sf::Uint8>(a.r + (b.r - a.r) * ratio),
				static_cast<sf::Uint8>(a.g + (b.g - a.g) * ratio),
				static_cast<sf::Uint8>(a.b + (b.b - a.b) * ratio)
	};
}

sf::Color Terrain::colorTempHumid(float temp, float humid)
{
	temp = std::min(temp, 1.f);
	temp = std::max(temp, -1.f);
	humid = std::min(humid, 1.f);
	humid = std::max(humid, 0.f);

	sf::Color warmMix{colorMix(warmDry, warmHumid, humid)};
	sf::Color extremeMix;
	if (temp > 0.f)
		extremeMix = colorMix(hotDry, hotHumid, humid);
	else
		extremeMix = colorMix(coldDry, coldHumid, humid);

	return colorMix(warmMix, extremeMix, std::abs(temp));
}

sf::Color Terrain::getColor(const std::vector<sf::Vector2f> &shape)
{
	float temp;
	get("temp") >> temp;
	float humid;
	get("humid") >> humid;

	static time_t master{time(0)};

	int rTotal{0}, gTotal{0}, bTotal{0};
	std::uniform_int_distribution<int> dist{-colorOffset, colorOffset};
	float screenHeight;
	get("screenHeight") >> screenHeight;
	for (auto &point : shape)
	{
		float tempNow{temp + (screenHeight - point.y) * heightDelta};

		sf::Color base{colorTempHumid(tempNow, humid)};
		rTotal += base.r;
		gTotal += base.g;
		bTotal += base.b;

		std::mt19937 generator{static_cast<unsigned int>(master) * (static_cast<unsigned int>(point.x) + static_cast<unsigned int>(point.y) * prime)};
		int randOffset{dist(generator)};
		rTotal += randOffset;
		gTotal += randOffset;
		bTotal += randOffset;
	}

	rTotal = std::min(rTotal, 3 * 255);
	gTotal = std::min(gTotal, 3 * 255);
	bTotal = std::min(bTotal, 3 * 255);
	rTotal = std::max(rTotal, 0);
	gTotal = std::max(gTotal, 0);
	bTotal = std::max(bTotal, 0);

	return {
				static_cast<sf::Uint8>((rTotal) / 3),
				static_cast<sf::Uint8>((gTotal) / 3),
				static_cast<sf::Uint8>((bTotal) / 3)
	       };
}

bool Terrain::check()
{
	if (!TriangleArray::check())
		return false;

	if (!db.count("temp"))
	{
		std::cerr << "variable \"temp\" not set" << std::endl;
		return false;
	}
	if (!db.count("humid"))
	{
		std::cerr << "variable \"humid\" not set" << std::endl;
		return false;
	}
	if (!db.count("minHeight"))
	{
		std::cerr << "variable \"minHeight\" not set" << std::endl;
		return false;
	}
	if (!db.count("maxHeight"))
	{
		std::cerr << "variable \"maxHeight\" not set" << std::endl;
		return false;
	}
	if (!db.count("screenWidth"))
	{
		std::cerr << "variable \"screenWidth\" not set" << std::endl;
		return false;
	}
	if (!db.count("screenHeight"))
	{
		std::cerr << "variable \"screenHeight\" not set" << std::endl;
		return false;
	}
	if (!db.count("dotCount"))
	{
		std::cerr << "variable \"dotCount\" not set" << std::endl;
		return false;
	}

	return true;
}

void Terrain::generate()
{
	std::lock_guard<std::mutex> guard{mutex};
	if (!check())
		return;

	clear();

	float minHeight;
	get("minHeight") >> minHeight;
	float maxHeight;
	get("maxHeight") >> maxHeight;
	Perlin perlin{perlinScale, minHeight, maxHeight};

	float screenWidth;
	get("screenWidth") >> screenWidth;
	float screenHeight;
	get("screenHeight") >> screenHeight;
	float dotCount;
	get("dotCount") >> dotCount;
	float width{screenWidth / dotCount};

	sf::Vector2f begin{0.f, screenHeight};
	polygon[0].push_back(begin);
	for (float x{0}; x <= dotCount; ++x)
	{
		sf::Vector2f vector{x * width, screenHeight - perlin.get(x * width)};
		polygon[0].push_back(vector);
	}

	sf::Vector2f end{dotCount * width, screenHeight};
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
