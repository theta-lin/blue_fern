#ifndef PERLIN_HPP
#define PERLIN_HPP

#include <SFML/Graphics.hpp>
#include <random>
#include <ctime>

class Perlin
{
private:
	const float m_scale;
	const time_t m_master;
	std::uniform_real_distribution<float> m_dist;

	float randHeight(unsigned int seed);
	float mix(float low, float high, float fraction);

public:
	Perlin() = delete;
	Perlin(float scale, float minHeight, float maxHeight)
		: m_scale{scale}, m_master{time(0)}, m_dist{minHeight, maxHeight}
	{
	}
	float get(float pos);
};

#endif