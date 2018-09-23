#include "perlin.hpp"

float Perlin::randHeight(unsigned int seed)
{
	std::mt19937 generator(static_cast<unsigned int>(m_master) * seed);
	return m_dist(generator);
}

float Perlin::mix(float low, float high, float fraction)
{
	float transit{fraction * fraction * fraction * (fraction * (fraction * 6 - 15) + 10)};
	return low + (high - low) * transit;
}

float Perlin::get(unsigned int pos)
{
	float i{static_cast<float>(pos / m_scale)};
	float f{static_cast<float>(pos % m_scale)};
	return mix(randHeight(i), randHeight(i + 1), f / m_scale);
}