#ifndef PERLIN_HPP
#define PERLIN_HPP

#include <ctime>
#include <random>

class Perlin
{
private:
	const unsigned int m_scale;
	const time_t m_master;
	std::uniform_real_distribution<float> m_dist;

	float randHeight(unsigned int seed);
	float mix(float low, float high, float fraction);

public:
	Perlin() = delete;
	Perlin(unsigned int scale, float minHeight, float maxHeight)
		: m_scale{scale}, m_master{time(0)}, m_dist{minHeight, maxHeight}
	{
	}

	float get(unsigned int pos);
};

#endif