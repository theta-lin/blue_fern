#include "perlin.hpp"
#include "linear.hpp"
#include <SFML/Graphics.hpp>
#include <vector>
#include <algorithm>
#include <random>
#include <limits>
#include <iostream>
#include <ctime>
#include <cmath>

const float count{35.f};
const float width{30.f};

void generate(std::vector<sf::Vector2f> &terrain)
{
	Perlin1D perlin{100, 50, 500};
	const float xOrigin{50.f}, yOrigin{1000.f}, ground{30};

	sf::Vector2f begin{xOrigin, yOrigin - ground};
	terrain.push_back(begin);
	for (unsigned int x{0}; x < count; ++x)
	{
		sf::Vector2f vector{xOrigin + x * width, yOrigin - perlin.get(x * width)};
		terrain.push_back(vector);
	}

	sf::Vector2f end{xOrigin + (count - 1) * width, yOrigin - ground};
	terrain.push_back(end);
	terrain.push_back(terrain[0]);
}

bool isPointInside(const sf::Vector2f &vec, const std::vector<sf::Vector2f> &polygon)
{
	Vector2D segment{sf::Vector2f(-1.f, -1.f), vec};

	int intCount(0);
	for (size_t i{1}; i < polygon.size(); ++i)
	{
		Vector2D edge{polygon[i - 1], polygon[i]};
		if (intersect(segment, edge))
			++intCount;
	}
	Vector2D bottom{polygon[0], polygon[polygon.size() - 1]};
	if (intersect(segment, bottom))
		++intCount;
	if (intCount % 2 == 1)
		return true;
	else
		return false;
}

bool isSegmentInside(const Vector2D segment, const std::vector<sf::Vector2f> &polygon)
{
	int intCount(0);
	for (size_t i{1}; i < polygon.size(); ++i)
	{
		Vector2D edge{polygon[i - 1], polygon[i]};
		if (intersect(segment, edge))
			++intCount;
	}
	Vector2D bottom{polygon[0], polygon[polygon.size() - 1]};
	if (intersect(segment, bottom))
		++intCount;
	return intCount < 2;
}

float getScore(const Vector2D e1, const Vector2D e2, const Vector2D e3, const Vector2D e4)
{
	const float perfectAngle{pi * 2 / 3};
	const float influence2(5.f);
	float angle1(angleVec(e1, e2)), angle2(angleVec(e2, e3)), angle3(angleVec(e3, e4));
	float avg((angle1 + angle2 + angle3) / 3);
	float diff1(std::abs(angle1 - avg) + std::abs(angle2 - avg) + std::abs(angle3 - avg));
	float diff2(std::abs(angleVec(e1, e4) - perfectAngle));
	return diff1 + diff2 * influence2;
}

void divide(std::vector<std::vector<sf::Vector2f> > &terrain)
{
	const float maxFloat{std::numeric_limits<float>::max()};
	const float maxArea{5000.f};
	const int maxIteration{20};

	bool done;
	do
	{
		done = true;
		for (size_t i(0); i < terrain.size(); ++i)
		{
			sf::Vector2f min{maxFloat, maxFloat}, max{0.f, 0.f};
			for (auto &vector : terrain[i])
			{
				min.x = std::min(min.x, vector.x);
				min.y = std::min(min.y, vector.y);
				max.x = std::max(max.x, vector.x);
				max.y = std::max(max.y, vector.y);
			}

			std::mt19937 generator(static_cast<unsigned int>(time(0)));
			std::uniform_real_distribution<float> distX{min.x, max.x}, distY{min.y, max.y};

			if (terrain[i].size() > 3)
			{ 
				done = false;
				// p0 is a random point inside the polygon
				sf::Vector2f p0;

				float minScore{maxFloat};
				int count{0};
				bool can;
				do
				{
					sf::Vector2f p0New{distX(generator), distY(generator)};;
					can = isPointInside(p0New, terrain[i]);

					// edges to the first 4 vertices of the polygon
					Vector2D p0p1 = {p0New, terrain[i][0]};
					Vector2D p0p2 = {p0New, terrain[i][1]};
					Vector2D p0p3 = {p0New, terrain[i][2]};
					Vector2D p0p4 = {p0New, terrain[i][3]};

					if (can)
					{
						can =    can
							  && isSegmentInside(p0p1, terrain[i])
							  && isSegmentInside(p0p2, terrain[i])
						  	  && isSegmentInside(p0p3, terrain[i])
							  && isSegmentInside(p0p4, terrain[i]);
					}

					if (can)
					{
						++count;
						float score{getScore(p0p1, p0p2, p0p3, p0p4)};
						if (score < minScore)
						{
							minScore = score;
							p0 = p0New;
						}
					}

				}
				while (!can || count < maxIteration);

				// create triangle p0p1p2, p0p2p3, p0p3p4
				std::vector<sf::Vector2f> p0p1p2{p0, terrain[i][0], terrain[i][1]};
				terrain.push_back(p0p1p2);
				std::vector<sf::Vector2f> p0p2p3{p0, terrain[i][1], terrain[i][2]};
				terrain.push_back(p0p2p3);
				std::vector<sf::Vector2f> p3p4p5{p0, terrain[i][2], terrain[i][3]};
				terrain.push_back(p3p4p5);

				//remaining polygon would be ... p1 p0 p4 ...
				terrain[i][1] = p0; // p2 = p0
				terrain[i].erase(terrain[i].begin() + 2); //erase p3
			}
		}
	}
	while (!done);
}

void draw(std::vector<std::vector<sf::Vector2f> > &triangle, sf::RenderWindow &window)
{
	int fill{0};
	for (auto &vertex : triangle)
	{
		sf::ConvexShape shape{3};
		shape.setPoint(0, vertex[0]);
		shape.setPoint(1, vertex[1]);
		shape.setPoint(2, vertex[2]);
		if (fill == 0)
			shape.setFillColor(sf::Color::Red);
		else if (fill == 1)
			shape.setFillColor(sf::Color::Yellow);
		else
			shape.setFillColor(sf::Color::Blue);
		fill = (fill + 1) % 3;
		window.draw(shape);
	}

	/*Perlin2D perlin{50, 0, 255};
	std::vector<sf::Vertex> point;
	for (float x{0.f}; x < 200.f; ++x)
	{
		for (float y{0.f}; y < 200.f; ++y)
		{
			sf::Vector2f vector{x, y};
			sf::Uint8 c{static_cast<sf::Uint8>(perlin.get(vector))};
			sf::Color color{c, c, c};
			point.push_back({vector, color});
		}
	}
	window.draw(point.data(), point.size(), sf::Points);*/

	return;
}

int main()
{
	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;
	sf::RenderWindow window(sf::VideoMode(1280, 1024), "hack_art", sf::Style::Default, settings);
	
	std::vector<std::vector<sf::Vector2f> > terrain;
	terrain.resize(1);
	generate(terrain[0]);
	divide(terrain);

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
