#include "cloud.hpp"

sf::Color Cloud::getColor(const std::vector<sf::Vector2f> &shape)
{
	static time_t master{time(0)};

	int total{static_cast<int>(3 * 255 * light)};
	std::uniform_int_distribution<int> dist{-colorOffset, colorOffset};
	for (auto &point : shape)
	{
		std::mt19937 generator{static_cast<unsigned int>(master) * (static_cast<unsigned int>(point.x) + static_cast<unsigned int>(point.y) * prime)};
		total += dist(generator);
	}

	total = std::min(total, 3 * 255);
	total = std::max(total, 0);

	return {
				static_cast<sf::Uint8>(total / 3),
				static_cast<sf::Uint8>(total / 3),
				static_cast<sf::Uint8>(total / 3),
				alpha
	};
}
