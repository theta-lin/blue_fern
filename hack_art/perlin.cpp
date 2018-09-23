#include "perlin.hpp"

float Perlin::randHeight(unsigned int seed)
{
	std::mt19937 generator{static_cast<unsigned int>(m_master) * seed};
	return m_dist(generator);
}

float Perlin::mix(float low, float high, float fraction)
{
	float transit{fraction * fraction * fraction * (fraction * (fraction * 6 - 15) + 10)};
	return low + (high - low) * transit;
}

float Perlin1D::get(float pos)
{
	pos /= m_scale;
	unsigned int i{static_cast<unsigned int>(pos)};
	float        f{pos - i};
	return mix(randHeight(i), randHeight(i + 1), f);
}

float Perlin2D::get(sf::Vector2f pos)
{
	pos.x /= m_scale;
	pos.y /= m_scale;
	unsigned int xi{static_cast<unsigned int>(pos.x)};
	float        xf{pos.x - xi};
	unsigned int yi{static_cast<unsigned int>(pos.y)};
	float        yf{pos.y - yi};

	/*
	 * +------------------+------------------+
	 * |     (xi, yi)     |   (xi + 1, yi)   |
	 * +------------------+------------------+
	 * |   (xi, yi + 1)   | (xi + 1, yi + 1) |
	 * +------------------+------------------+
	 */
	float x1{mix(randHeight(xi * yi), randHeight((xi + 1) * yi), xf)};
	float x2{mix(randHeight(xi * (yi + 1)), randHeight((xi + 1) * (yi + 1)), xf)};
	return mix(x1, x2, yf);
}
