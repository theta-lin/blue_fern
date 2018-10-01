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
	Perlin perlin{150, 100, 600};
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
	const int maxIntersect{3};
	int intCount{0};
	for (size_t i{1}; i < polygon.size(); ++i)
	{
		Vector2D edge{polygon[i - 1], polygon[i]};
		if (intersect(segment, edge))
			++intCount;
	}
	Vector2D bottom{polygon[0], polygon[polygon.size() - 1]};
	if (intersect(segment, bottom))
		++intCount;
	return intCount < maxIntersect;
}

float getScore(const Vector2D e1, const Vector2D e2, const Vector2D e3, const Vector2D e4)
{
	const float perfectAngle{pi * 4 / 5};
	const float influence2(5.f);
	float angle1(angleVec(e1, e2)), angle2(angleVec(e2, e3)), angle3(angleVec(e3, e4));
	float avg((angle1 + angle2 + angle3) / 3);
	float diff1(std::abs(angle1 - avg) + std::abs(angle2 - avg) + std::abs(angle3 - avg));
	float diff2(std::abs(angleVec(e1, e4) - perfectAngle));
	return diff1 + diff2 * influence2;
}

std::vector<std::vector<sf::Vector2f> > divide(std::vector<sf::Vector2f> &terrain)
{
	const float maxFloat{std::numeric_limits<float>::max()};
	const float maxArea{5000.f};
	const int maxIteration{20};

	std::vector<std::vector<sf::Vector2f> > triangles;
	triangles.push_back(terrain);

	std::mt19937 generator(static_cast<unsigned int>(time(0)));
	while (triangles[0].size() > 3)
	{
		sf::Vector2f min{maxFloat, maxFloat}, max{0.f, 0.f};
		for (size_t j{0}; j < triangles[0].size(); ++j)
		{
			min.x = std::min(min.x, triangles[0][j].x);
			min.y = std::min(min.y, triangles[0][j].y);
			max.x = std::max(max.x, triangles[0][j].x);
			max.y = std::max(max.y, triangles[0][j].y);
		}

		std::uniform_real_distribution<float> distX{min.x, max.x}, distY{min.y, max.y};
		std::uniform_int_distribution<size_t> distOffset{0, 3};
		size_t offset{distOffset(generator)};
		sf::Vector2f p0{triangles[0][offset]};
		sf::Vector2f p1{triangles[0][(offset + 1) % triangles[0].size()]};
		sf::Vector2f p2{triangles[0][(offset + 2) % triangles[0].size()]};
		sf::Vector2f p3{triangles[0][(offset + 3) % triangles[0].size()]};

		// p is a random point inside the polygon
		sf::Vector2f p;
		float minScore{maxFloat};
		int count{0};
		bool can;
		do
		{
			sf::Vector2f pNew{distX(generator), distY(generator)};;
			can = isPointInside(pNew, triangles[0]);

			// edges to the first 4 vertices of the polygon
			Vector2D pp0 = {pNew, p0};
			Vector2D pp1 = {pNew, p1};
			Vector2D pp2 = {pNew, p2};
			Vector2D pp3 = {pNew, p3};

			if (can)
			{
				can =    can
						&& isSegmentInside(pp0, triangles[0])
						&& isSegmentInside(pp1, triangles[0])
					  	&& isSegmentInside(pp2, triangles[0])
						&& isSegmentInside(pp3, triangles[0]);
			}

			if (can)
			{
				++count;
				float score{getScore(pp0, pp1, pp2, pp3)};
				if (score < minScore)
				{
					minScore = score;
					p = pNew;
				}
			}
		}
		while (!can || count < maxIteration);

		// create triangle pp0p1, pp1p2, pp2p3
		std::vector<sf::Vector2f> pp0p1{p, p0, p1};
		triangles.push_back(pp0p1);
		std::vector<sf::Vector2f> pp1p2{p, p1, p2};
		triangles.push_back(pp1p2);
		std::vector<sf::Vector2f> pp2p3{p, p2, p3};
		triangles.push_back(pp2p3);

		//remaining polygon would be ... p0 p p3 ...
		triangles[0][(offset + 1) % triangles[0].size()] = p; // p1 = p
		triangles[0].erase(triangles[0].begin() + (offset + 2) % triangles[0].size()); //erase p2
	}

	return triangles;
}

sf::Color getColor(const sf::ConvexShape &shape)
{
	static time_t master{time(0)};
	std::uniform_real_distribution<float> dist{32, 255};

	const unsigned int prime{100003};
	float sum{0.f};
	for (int v(0); v < 3; ++v)
	{
		sf::Vector2f point{shape.getPoint(v)};
		std::mt19937 generator{static_cast<unsigned int>(master) * (static_cast<unsigned int>(point.x) + static_cast<unsigned int>(point.y) * prime)};
		sum += dist(generator);
	}
	sf::Color color{0, static_cast<sf::Uint8>(sum / 3.f), 0};
	return color;
}

void draw(std::vector<std::vector<sf::Vector2f> > &triangle, sf::RenderWindow &window)
{
	for (auto &vertex : triangle)
	{
		sf::ConvexShape shape{3};
		shape.setPoint(0, vertex[0]);
		shape.setPoint(1, vertex[1]);
		shape.setPoint(2, vertex[2]);
		shape.setFillColor(getColor(shape));
		window.draw(shape);
	}

	return;
}

int main()
{
	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;
	sf::RenderWindow window(sf::VideoMode(1280, 1024), "hack_art", sf::Style::Default, settings);
	
	std::vector<sf::Vector2f> terrain;
	generate(terrain);
	std::vector<std::vector<sf::Vector2f> > triangles{divide(terrain)};

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		window.clear(sf::Color::Black);
		draw(triangles, window);
		window.display();
	}
	return 0;
}
