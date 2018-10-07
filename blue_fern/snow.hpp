#ifndef SNOW_HPP
#define SNOW_HPP

#include "object.hpp"
#include "linear.hpp"
#include <iostream>
#include <vector>

class Snow : public Object
{
	/*
	 * Require:
	 * radius (default)
	 * iteration (default)
	 * xOrigin
	 * yOrigin
	 */

private:
	const sf::Color color{255, 255, 255, 160};

	void iterate(int count, sf::Vector2f origin, float radius, float angle);

	bool check() final;
	void clear() final;

	std::vector<sf::Vertex> vertices;

public:
	Snow() : Object()
	{
		set("radius", "30");
		set("iteration", "5");
	}

	void generate() final;
	void draw(sf::RenderTarget &target) final;
	bool hasShadow() final { return true; }
	sf::Vector2f getPos() final
	{
		sf::Vector2f pos;
		get("xOrigin") >> pos.x;
		get("xOrigin") >> pos.y;
		return pos;
	}
	std::unique_ptr<Object> clone() override { return std::make_unique<Snow>(*this); }
};

#endif
