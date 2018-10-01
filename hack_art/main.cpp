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
const float width{35.f};
const float xOrigin{50.f}, yOrigin{1000.f}, ground{30};

void generate(std::vector<sf::Vector2f> &terrain)
{
	Perlin perlin{150, 100, 600};

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
	const int maxIntersect{1};
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
	return intCount <= maxIntersect;
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

const float maxFloat{std::numeric_limits<float>::max()};

void dividePolygon(std::vector<std::vector<sf::Vector2f> > &triangles)
{
	const int maxIteration{20};
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

		std::uniform_int_distribution<size_t> distOffset{0, 3};
		size_t offset{distOffset(generator)};
		const sf::Vector2f &p0{triangles[0][offset]};
		const sf::Vector2f &p1{triangles[0][(offset + 1) % triangles[0].size()]};
		const sf::Vector2f &p2{triangles[0][(offset + 2) % triangles[0].size()]};
		const sf::Vector2f &p3{triangles[0][(offset + 3) % triangles[0].size()]};

		// p is a random point inside the polygon
		sf::Vector2f p;
		float minScore{maxFloat};
		std::uniform_real_distribution<float> distX{min.x, max.x}, distY{min.y, max.y};

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
				can = can
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
		} while (!can || count < maxIteration);

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
}

void divideTriangles(std::vector<std::vector<sf::Vector2f> > &triangles)
{
	const float maxEdge{50.f};

	for (size_t i{0}; i < triangles.size(); ++i)
	{
		sf::Vector2f *p0{nullptr};
		sf::Vector2f *p1{nullptr};
		sf::Vector2f *p2{nullptr};

		//find the longest edge of the triangle (oppositing p0)
		for (size_t offset{0}; offset < 3; ++offset)
		{
			sf::Vector2f *p0New{&triangles[i][offset]};
			sf::Vector2f *p1New{&triangles[i][(offset + 1) % 3]};
			sf::Vector2f *p2New{&triangles[i][(offset + 2) % 3]};
			if (!p0 || getMag({*p1New, *p2New}) > getMag({*p1, *p2}))
			{
				p0 = p0New;
				p1 = p1New;
				p2 = p2New;
			}
		}
			
		//if that edge is too long, connect p0 with its midpoint
		if (getMag({*p1, *p2}) > maxEdge)
		{
			sf::Vector2f mid = {(p1->x + p2->x) / 2, (p1->y + p2->y) / 2};
			triangles.insert(triangles.begin() + i + 1, {*p0, mid, *p2});
			*p2 = mid;
		}
	}

}

std::vector<std::vector<sf::Vector2f> > divide(const std::vector<sf::Vector2f> &polygon)
{
	std::vector<std::vector<sf::Vector2f> > triangles;
	triangles.push_back(polygon);
	dividePolygon(triangles);
	divideTriangles(triangles);
	return triangles;
}

sf::Color colorMix(const sf::Color &a, const sf::Color &b, const float ratio)
{
	return {
				static_cast<sf::Uint8>(a.r + (b.r - a.r) * ratio),
				static_cast<sf::Uint8>(a.g + (b.g - a.g) * ratio),
				static_cast<sf::Uint8>(a.b + (b.b - a.b) * ratio)
	       };
}

sf::Color colorTempHumid(float temp, float humid)
{
	temp = std::min(temp, 1.f);
	temp = std::max(temp, -1.f);
	humid = std::min(humid, 1.f);
	humid = std::max(humid, 0.f);

	const sf::Color hotDry   {255, 0,   0  };
	const sf::Color warmDry  {255, 255, 0  };
	const sf::Color coldDry  {180, 220, 255};
	const sf::Color hotHumid {0,   128, 0  };
	const sf::Color warmHumid{0,   255, 0  };
	const sf::Color coldHumid{100, 255, 255};

	sf::Color warmMix{colorMix(warmDry, warmHumid, humid)};
	sf::Color extremeMix;
	if (temp > 0.f)
		extremeMix = colorMix(hotDry, hotHumid, humid);
	else
		extremeMix = colorMix(coldDry, coldHumid, humid);

	return colorMix(warmMix, extremeMix, std::abs(temp));
}

sf::Color getColor(const sf::ConvexShape &shape)
{
	const float temp{1.f};
	const float humid{1.f};
	const float delta{-0.004f};
	static time_t master{time(0)};

	const unsigned int prime{100003};
	float rTotal{0}, gTotal{0}, bTotal{0};
	for (int v(0); v < 3; ++v)
	{
		sf::Vector2f point{shape.getPoint(v)};
		float tempNow{temp + (yOrigin - ground - point.y) * delta};

		sf::Color base{colorTempHumid(tempNow, humid)};
		rTotal += static_cast<float>(base.r);
		gTotal += static_cast<float>(base.g);
		bTotal += static_cast<float>(base.b);
		std::mt19937 generator{static_cast<unsigned int>(master) * (static_cast<unsigned int>(point.x) + static_cast<unsigned int>(point.y) * prime)};
		std::uniform_real_distribution<float> dist{-50.f, 50.f};
		//std::cout << rTotal << ' ' << randOffset << '\n';
		//system("pause");
		float randOffset{dist(generator)};
		rTotal += randOffset;
		gTotal += randOffset;
		bTotal += randOffset;
	}

	rTotal = std::min(rTotal, 3.f * 255.f);
	gTotal = std::min(gTotal, 3.f * 255.f);
	bTotal = std::min(bTotal, 3.f * 255.f);
	rTotal = std::max(rTotal, 0.f);
	gTotal = std::max(gTotal, 0.f);
	bTotal = std::max(bTotal, 0.f);

	//std::cout << static_cast<int>((rTotal + 10) / 3.f) << ' ' << static_cast<int>((gTotal) / 3.f) << ' ' << static_cast<int>((bTotal) / 3.f) << std::endl;
	//system("pause");

	return {
				static_cast<sf::Uint8>((rTotal) / 3.f),
				static_cast<sf::Uint8>((gTotal) / 3.f),
				static_cast<sf::Uint8>((bTotal) / 3.f)
	       };
}

void draw(std::vector<std::vector<sf::Vector2f> > &triangle, sf::RenderWindow &window)
{
	for (auto &vertex : triangle)
	{
		sf::ConvexShape shape{3};
		shape.setPoint(0, vertex[0]);
		shape.setPoint(1, vertex[1]);
		shape.setPoint(2, vertex[2]);
		sf::Color www = getColor(shape);
		//std::cout << int(www.r) << ' ' << int(www.g) << ' ' << int(www.b) << std::endl;
		//system("pause");
		shape.setFillColor(www);
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
