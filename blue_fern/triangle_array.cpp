#include "triangle_array.hpp"

float TriangleArray::getScore(const Vector2D e1, const Vector2D e2, const Vector2D e3, const Vector2D e4)
{
	float perfectAngle;
	get("perfectAngle") >> perfectAngle;
	perfectAngle *= pi;

	float influence1;
	get("influence1") >> influence1;
	float influence2;
	get("influence2") >> influence2;

	float angle1(angleVec(e1, e2)), angle2(angleVec(e2, e3)), angle3(angleVec(e3, e4));
	float avg((angle1 + angle2 + angle3) / 3);
	float diff1(std::abs(angle1 - avg) + std::abs(angle2 - avg) + std::abs(angle3 - avg));
	float diff2(std::abs(angleVec(e1, e4) - perfectAngle));

	return diff1 * influence1 + diff2 * influence2;
}

void TriangleArray::dividePolygon()
{
	int maxIteration;
	get("maxIteration") >> maxIteration;
	int maxIntersect;
	get("maxIntersect") >> maxIntersect;

	std::mt19937 generator(static_cast<unsigned int>(time(0)));
	std::uniform_int_distribution<size_t> distOffset{0, 3};
	while (polygon[0].size() > 3)
	{
		sf::Vector2f min{maxFloat, maxFloat}, max{0.f, 0.f};
		for (size_t j{0}; j < polygon[0].size(); ++j)
		{
			min.x = std::min(min.x, polygon[0][j].x);
			min.y = std::min(min.y, polygon[0][j].y);
			max.x = std::max(max.x, polygon[0][j].x);
			max.y = std::max(max.y, polygon[0][j].y);
		}

		// p is a random point inside the polygon
		sf::Vector2f p;
		float minScore{maxFloat};
		std::uniform_real_distribution<float> distX{min.x, max.x}, distY{min.y, max.y};

		// p0, p1, p2, p3 are 4 consective vertices of the polygon
		size_t offset{distOffset(generator)};
		const sf::Vector2f &p0{polygon[0][offset]};
		const sf::Vector2f &p1{polygon[0][(offset + 1) % polygon[0].size()]};
		const sf::Vector2f &p2{polygon[0][(offset + 2) % polygon[0].size()]};
		const sf::Vector2f &p3{polygon[0][(offset + 3) % polygon[0].size()]};

		int count{0};
		bool can;
		do
		{
			sf::Vector2f pNew{distX(generator), distY(generator)};;
			can = isPointInside(pNew, polygon[0]);

			// edges to the 4 vertices of the polygon
			Vector2D pp0 = {pNew, p0};
			Vector2D pp1 = {pNew, p1};
			Vector2D pp2 = {pNew, p2};
			Vector2D pp3 = {pNew, p3};

			if (can)
			{
				can = can
					&& isSegmentInside(pp0, polygon[0], maxIntersect)
					&& isSegmentInside(pp1, polygon[0], maxIntersect)
					&& isSegmentInside(pp2, polygon[0], maxIntersect)
					&& isSegmentInside(pp3, polygon[0], maxIntersect);
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
		polygon.push_back(pp0p1);
		std::vector<sf::Vector2f> pp1p2{p, p1, p2};
		polygon.push_back(pp1p2);
		std::vector<sf::Vector2f> pp2p3{p, p2, p3};
		polygon.push_back(pp2p3);

		//remaining polygon would be ... p0 p p3 ...
		polygon[0][(offset + 1) % polygon[0].size()] = p; // p1 = p
		polygon[0].erase(polygon[0].begin() + (offset + 2) % polygon[0].size()); //erase p2
	}
}

void TriangleArray::divideTriangles()
{
	float maxEdge;
	get("maxEdge") >> maxEdge;

	for (size_t i{0}; i < polygon.size(); ++i)
	{
		sf::Vector2f *p0{nullptr};
		sf::Vector2f *p1{nullptr};
		sf::Vector2f *p2{nullptr};

		//find the longest edge of the triangle (oppositing p0)
		for (size_t offset{0}; offset < 3; ++offset)
		{
			sf::Vector2f *p0New{&polygon[i][offset]};
			sf::Vector2f *p1New{&polygon[i][(offset + 1) % 3]};
			sf::Vector2f *p2New{&polygon[i][(offset + 2) % 3]};
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
			polygon.insert(polygon.begin() + i + 1, {*p0, mid, *p2});
			*p2 = mid;
		}
	}

}

void TriangleArray::draw(sf::RenderTarget &target)
{
	if (canDraw)
			target.draw(vertices.data(), vertices.size(), sf::Triangles);
}

bool TriangleArray::check()
{
	if (!db.count("perfectAngle"))
	{
		std::cerr << "variable \"perfectAngle\" not set" << std::endl;
		return false;
	}
	if (!db.count("influence1"))
	{
		std::cerr << "variable \"influence1\" not set" << std::endl;
		return false;
	}
	if (!db.count("influence2"))
	{
		std::cerr << "variable \"influence2\" not set" << std::endl;
		return false;
	}
	if (!db.count("maxIteration"))
	{
		std::cerr << "variable \"maxIteration\" not set" << std::endl;
		return false;
	}
	if (!db.count("maxIntersect"))
	{
		std::cerr << "variable \"maxIntersect\" not set" << std::endl;
		return false;
	}
	if (!db.count("maxEdge"))
	{
		std::cerr << "variable \"maxEdge\" not set" << std::endl;
		return false;
	}

	return true;
}
