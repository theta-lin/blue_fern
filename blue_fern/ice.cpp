#include "ice.hpp"

sf::Color Ice::getColor(const std::vector<sf::Vector2f> &shape)
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
