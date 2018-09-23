#include <SFML/Graphics.hpp>
#include <vector>
#include "perlin.hpp"
#include <iostream>

const float count{20.f};
const float width{50.f};

void calculate(std::vector<sf::Vertex> &terrain)
{
	Perlin perlin{80, 50, 500};
	const float xOrigin{50.f}, yOrigin{1000.f};
	for (unsigned int x{0}; x < count; ++x)
	{
		sf::Vertex vertex{sf::Vector2f{xOrigin + x * width, yOrigin - perlin.get(x * width)}, sf::Color::Green};
		terrain.push_back(vertex);
	}
}

void draw(std::vector<sf::Vertex> &terrain, sf::RenderWindow &window)
{
	window.draw(terrain.data(), terrain.size(), sf::LineStrip);

	sf::ConvexShape convex;
	convex.setFillColor(sf::Color::Cyan);
	convex.setPointCount(5);
	convex.setPoint(0, sf::Vector2f(50, 50));
	convex.setPoint(1, sf::Vector2f(700, 50));
	convex.setPoint(2, sf::Vector2f(600, 400));
	convex.setPoint(3, sf::Vector2f(100, 500));
	convex.setPoint(4, sf::Vector2f(0, 200));
	window.draw(convex);
	return;
}

int main()
{
	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;
	sf::RenderWindow window(sf::VideoMode(1280, 1024), "hack_art", sf::Style::Default, settings);
	std::vector<sf::Vertex> terrain;
	calculate(terrain);

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		window.clear(sf::Color::Black);
		draw(terrain, window);
		window.display();
	}
	return 0;
}
