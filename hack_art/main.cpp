#include "perlin.hpp"
#include "linear.hpp"
#include <SFML/Graphics.hpp>
#include <vector>
#include <algorithm>
#include <random>
#include <limits>
#include <iostream>
#include <ctime>

const float count{20.f};
const float width{50.f};

void generate(std::vector<sf::Vector2f> &terrain)
{
	Perlin perlin{80, 50, 500};
	const float xOrigin{50.f}, yOrigin{1000.f}, ground{30};

	sf::Vector2f begin{xOrigin, yOrigin - ground};
	terrain.push_back(begin);
	for (unsigned int x{0}; x < count; ++x)
	{
		sf::Vector2f vector{xOrigin + x * width, yOrigin - perlin.get(static_cast<unsigned int>(x * width))};
		terrain.push_back(vector);
	}

	sf::Vector2f end{xOrigin + (count - 1) * width, yOrigin - ground};
	terrain.push_back(end);
	terrain.push_back(terrain[0]);
}

bool isInside(const sf::Vector2f &vec, std::vector<sf::Vector2f> &terrain)
{
	Vector2D segment{sf::Vector2f(-1.f, -1.f), vec};

	int intCount(0);
	for (size_t i{1}; i < terrain.size(); ++i)
	{
		Vector2D edge{terrain[i - 1], terrain[i]};
		if (intersect(segment, edge))
			++intCount;
	}
	Vector2D bottom{terrain[0], terrain[terrain.size() - 1]};
	if (intersect(segment, bottom))
		++intCount;

	if (intCount % 2 == 1)
		return true;
	else
		return false;
}

void plot(std::vector<sf::Vector2f> &terrain, std::vector<sf::Vertex> &point)
{
	const float maxFloat{std::numeric_limits<float>::max()};
	sf::Vector2f min{maxFloat, maxFloat}, max{0.f, 0.f};
	for (auto &vector : terrain)
	{
		min.x = std::min(min.x, vector.x);
		min.y = std::min(min.y, vector.y);
		max.x = std::max(max.x, vector.x);
		max.y = std::max(max.y, vector.y);
	}

	std::mt19937 generator(static_cast<unsigned int>(time(0)));
	std::uniform_real_distribution<float> distX{min.x, max.x}, distY{min.y, max.y};
	for (int i{0}; i < 10000; ++i)
	{
		sf::Vector2f vec{distX(generator), distY(generator)};
		if (isInside(vec, terrain))
			point.push_back(sf::Vertex(vec, sf::Color::Green));
		else
			point.push_back(sf::Vertex(vec, sf::Color::Red));
	}
}

void draw(std::vector<sf::Vector2f> &terrain, std::vector<sf::Vertex> &point, sf::RenderWindow &window)
{
	std::vector<sf::Vertex> polygon;
	for (auto &vector : terrain)
		polygon.push_back(sf::Vertex{vector, sf::Color::Magenta});
	window.draw(polygon.data(), polygon.size(), sf::LineStrip);
	window.draw(point.data(), point.size(), sf::Points);
	return;
}

int main()
{
	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;
	sf::RenderWindow window(sf::VideoMode(1280, 1024), "hack_art", sf::Style::Default, settings);
	
	std::vector<sf::Vector2f> terrain;
	generate(terrain);
	std::vector<sf::Vertex> point;
	plot(terrain, point);

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		window.clear(sf::Color::Black);
		draw(terrain, point, window);
		window.display();
	}
	return 0;
}
