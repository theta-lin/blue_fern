#include "terrain.hpp"
#include "cloud.hpp"
#include "water.hpp"
#include "ice.hpp"
#include "fern.hpp"
#include "tree.hpp"
#include "red_tree.hpp"
#include "snow.hpp"

#include "linear.hpp"

#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <memory>
#include <functional>
#include <thread>
#include <mutex>

const unsigned int screenWidth{1920};
const unsigned int screenHeight{1080};
const sf::Color shadowColor{100, 100, 100, 220};
const float shadowOffset{5.f};
const float sunRadius{30.f};

bool g_end{false};
std::mutex g_mutexEnd;

sf::Color g_skyColor{0, 150, 230};
sf::Vector2f g_sunPos{-100.f, -100.f};
std::vector<std::unique_ptr<Object> > g_queue;
std::vector<std::string> g_id;
std::mutex g_mutexObject;

sf::RenderTexture g_screen;
std::mutex g_mutexScreen;

void add(const std::string &name, const std::string &type)
{
	if (std::find(g_id.begin(), g_id.end(), name) != g_id.end())
	{
		std::cerr << "Name already used!" << std::endl;
	}
	else
	{
		if (type == "terrain")
		{
			g_queue.push_back(std::make_unique<Terrain>());
			g_queue.back()->set("screenWidth", std::to_string(screenWidth));
			g_queue.back()->set("screenHeight", std::to_string(screenHeight));
			g_id.push_back(name);
		}
		else if (type == "cloud")
		{
			g_queue.push_back(std::make_unique<Cloud>());
			g_id.push_back(name);
		}
		else if (type == "water")
		{
			g_queue.push_back(std::make_unique<Water>());
			g_id.push_back(name);
		}
		else if (type == "ice")
		{
			g_queue.push_back(std::make_unique<Ice>());
			g_id.push_back(name);
		}
		else if (type == "fern")
		{
			g_queue.push_back(std::make_unique<Fern>());
			g_id.push_back(name);
		}
		else if (type == "tree")
		{
			g_queue.push_back(std::make_unique<Tree>());
			g_id.push_back(name);
		}
		else if (type == "red_tree")
		{
			g_queue.push_back(std::make_unique<RedTree>());
			g_id.push_back(name);
		}
		else if (type == "snow")
		{
			g_queue.push_back(std::make_unique<Snow>());
			g_id.push_back(name);
		}
		else
		{
			std::cerr << "Undefined object!" << std::endl;
		}
	}
}

void remove(const std::string &name)
{
	bool removed{false};
	for (size_t i{0}; i < g_id.size(); ++i)
	{
		if (g_id[i] == name)
		{
			g_queue.erase(g_queue.begin() + i);
			g_id.erase(g_id.begin() + i);
			removed = true;
			break;
		}
	}

	if (!removed)
		std::cerr << "Object not found!" << std::endl;
}

void set(const std::string &name, const std::string &key, const std::string &value)
{
	size_t index;
	bool found{false};
	for (size_t i{0}; i < g_id.size(); ++i)
	{
		if (g_id[i] == name)
		{
			index = i;
			found = true;
			break;
		}
	}

	if (!found)
		std::cerr << "Object not found" << std::endl;
	else
		g_queue[index]->set(key, value);
}

void generate(const std::string &name)
{
	size_t index;
	bool found{false};
	for (size_t i{0}; i < g_id.size(); ++i)
	{
		if (g_id[i] == name)
		{
			index = i;
			found = true;
			break;
		}
	}

	if (!found)
		std::cerr << "Object not found" << std::endl;
	else
		g_queue[index]->generate();
}

void update(const std::string &name)
{
	size_t index;
	bool found{false};
	for (size_t i{0}; i < g_id.size(); ++i)
	{
		if (g_id[i] == name)
		{
			index = i;
			found = true;
			break;
		}
	}

	if (!found)
		std::cerr << "Object not found" << std::endl;
	else
		g_queue[index]->update();
}

void down(const std::string &name)
{
	size_t index;
	bool found{false};
	for (size_t i{0}; i < g_id.size(); ++i)
	{
		if (g_id[i] == name)
		{
			index = i;
			found = true;
			break;
		}
	}

	if (!found)
	{
		std::cerr << "Object not found" << std::endl;
	}
	else
	{
		if (index != 0)
		{
			std::swap(g_queue[index], g_queue[index - 1]);
			std::swap(g_id[index], g_id[index - 1]);
		}
	}
}

void duplicate(const std::string &name, const std::string &newName)
{
	if (std::find(g_id.begin(), g_id.end(), newName) != g_id.end())
	{
		std::cerr << "Name already used!" << std::endl;
	}
	else
	{
		size_t index;
		bool found{false};
		for (size_t i{0}; i < g_id.size(); ++i)
		{
			if (g_id[i] == name)
			{
				index = i;
				found = true;
				break;
			}
		}

		if (!found)
		{
			std::cerr << "Object not found" << std::endl;
		}
		else
		{
			g_queue.push_back(std::unique_ptr<Object>{g_queue[index]->clone()});
			g_id.push_back(newName);
		}
	}
}

void control(sf::RenderWindow &window)
{
	std::string line;
	while (true)
	{
		std::getline(std::cin, line);
		std::lock_guard<std::mutex> guard(g_mutexObject);

		std::istringstream stream{line};
		std::string command;
		stream >> command;

		if (command.empty())
		{
			std::cerr << "Command not given!" << std::endl;
		}
		else if (command == "close")
		{
			g_mutexEnd.lock();
			g_end = true;
			g_mutexEnd.unlock();
			break;
		}
		else if (command == "list")
		{
			for (auto &id : g_id)
				std::cout << id << ' ';
			std::cout << std::endl;
		}
		else if (command == "sky")
		{
			int r, g, b;
			stream >> r >> g >> b;
			g_skyColor.r = static_cast<sf::Uint8>(r);
			g_skyColor.g = static_cast<sf::Uint8>(g);
			g_skyColor.b = static_cast<sf::Uint8>(b);
		}
		else if (command == "sun")
		{
			stream >> g_sunPos.x >> g_sunPos.y;
		}
		else if (command == "save")
		{
			std::string file;
			stream >> file;
			if (file.empty())
				std::cerr << "No file name!" << std::endl;
			else
				g_screen.getTexture().copyToImage().saveToFile(file);
		}
		else
		{
			std::string name;
			stream >> name;
			if (command == "add")
			{
				if (name.empty())
				{
					std::cerr << "Name not set!" << std::endl;
				}
				else
				{
					std::string type;
					stream >> type;
					if (type.empty())
					{
						std::cerr << "Type not set!" << std::endl;
					}
					else
					{
						add(name, type);
					}
				}

			}
			else if (command == "remove")
			{
				if (name.empty())
				{
					std::cerr << "Name not set!" << std::endl;
				}
				else
				{
					remove(name);
				}
			}
			else if (command == "set")
			{
				if (name.empty())
				{
					std::cerr << "Name not set!" << std::endl;
				}
				else
				{
					std::string key;
					stream >> key;
					if (key.empty())
					{
						std::cerr << "Key not set!" << std::endl;
					}
					else
					{
						std::string value;
						stream >> value;
						if (value.empty())
						{
							std::cerr << "Value not set!" << std::endl;
						}
						else
						{
							set(name, key, value);
						}
					}
				}
			}
			else if (command == "gen")
			{
				if (name.empty())
				{
					std::cerr << "Name not set!" << std::endl;
				}
				else
				{
					generate(name);
				}
			}
			else if (command == "update")
			{
				if (name.empty())
				{
					std::cerr << "Name not set!" << std::endl;
				}
				else
				{
					update(name);
				}
			}
			else if (command == "down")
			{
				if (name.empty())
				{
					std::cerr << "Name not set!" << std::endl;
				}
				else
				{
					down(name);
				}
			}
			else if (command == "dup")
			{
				if (name.empty())
				{
					std::cerr << "Name not set!" << std::endl;
				}
				else
				{
					std::string newName;
					stream >> newName;
					if (newName.empty())
					{
						std::cerr << "New Name not set!" << std::endl;
					}
					else
					{
						duplicate(name, newName);
					}
				}
			}
			else
			{
				std::cerr << "Command not found!" << std::endl;
			}
		}
	}
}

int main()
{
	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;
	sf::RenderWindow window{sf::VideoMode{screenWidth, screenHeight}, "blue_fern", sf::Style::Titlebar, settings};

	std::thread controlThread{control, std::ref(window)};

	if (!g_screen.create(screenWidth, screenHeight, settings))
		return 1;

	while (true)
	{
		bool end;
		g_mutexEnd.lock();
		end = g_end;
		g_mutexEnd.unlock();

		if (end)
		{
			window.close();
			break;
		}
		else
		{
			g_mutexScreen.lock();
			g_screen.clear(sf::Color::Black);

			g_mutexObject.lock();
			sf::RectangleShape sky{sf::Vector2f{static_cast<float>(screenWidth), static_cast<float>(screenHeight)}};
			sky.setFillColor(g_skyColor);
			g_screen.draw(sky);
			sf::CircleShape sun{sunRadius};
			sun.setPosition(g_sunPos);
			sun.setFillColor(sf::Color{255,255,255});
			g_screen.draw(sun);

			for (auto &object : g_queue)
			{
				sf::RenderTexture render;
				if (render.create(screenWidth, screenHeight, settings))
				{
					render.clear(sf::Color{0, 0, 0, 0});
					object->draw(render);
					render.display();

					const sf::Texture &texture{render.getTexture()};
					if (object->hasShadow())
					{
						sf::Sprite shadow{texture};
						shadow.setColor(shadowColor);
						sf::Vector2f pos{object->getPos()};
						shadow.move(move({0, 0}, shadowOffset, std::atan2(pos.y - g_sunPos.y, pos.x - g_sunPos.x)));
						g_screen.draw(shadow);
					}

					sf::Sprite shape{texture};
					g_screen.draw(shape);
				}
			}
			g_mutexObject.unlock();

			g_screen.display();
			const sf::Texture &texture{g_screen.getTexture()};
			sf::Sprite screen{texture};
			
			sf::Event event;
			while (window.pollEvent(event))
				;
			window.clear(sf::Color::Black);
			window.draw(screen);
			g_mutexScreen.unlock();
			window.display();
		}
	}

	controlThread.join();
	return 0;
}
