#ifndef PERLIN_HPP
#define PERLIN_HPP

#include <SFML/Graphics.hpp>
#include <random>
#include <ctime>

class Perlin
{
protected:
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
};

class Perlin1D : public Perlin
{
public:
	Perlin1D() = delete;
	using Perlin::Perlin;
	float get(float pos);
};

class Perlin2D : public Perlin
{
private:
	const unsigned int prime;
public:
	Perlin2D() = delete;
	Perlin2D(float scale, float minHeight, float maxHeight)
		: Perlin::Perlin(scale, minHeight, maxHeight), prime{100003}
	{
	}
	float get(sf::Vector2f pos);
};

#endif