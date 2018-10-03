#include "terrain.hpp"
#include <SFML/Graphics.hpp>

int main()
{
	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;
	sf::RenderWindow window(sf::VideoMode(1280, 1024), "hack_art", sf::Style::Default, settings);
	Terrain terrain;
	terrain.set("temp", "0");
	terrain.set("humid", "0.5");
	terrain.set("minHeight", "200");
	terrain.set("maxHeight", "800");
	terrain.set("screenWidth", "1280");
	terrain.set("screenHeight", "1024");
	terrain.set("dotCount", "40");
	terrain.generate();

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}
		
		window.clear(sf::Color::Black);
		terrain.draw(window);
		window.display();
	}
	return 0;
}
