#include "snow.hpp"

void Snow::iterate(int count, sf::Vector2f origin, float radius, float angle)
{
	int max;
	get("iteration") >> max;
	if (count < max)
	{
		vertices.push_back({move(origin, radius, angle), color});
		vertices.push_back({move(origin, radius, angle + pi / 3.f * 2.f), color});
		vertices.push_back({move(origin, radius, angle + pi / 3.f * 4.f), color});

		iterate(count + 1, move(origin, radius / 4.f * 3.f, angle), radius / 2.f, angle);
		iterate(count + 1, move(origin, radius / 4.f * 3.f, angle + pi / 3.f * 2.f), radius / 2.f, angle + pi / 3.f * 2.f);
		iterate(count + 1, move(origin, radius / 4.f * 3.f, angle + pi / 3.f * 4.f), radius / 2.f, angle + pi / 3.f * 4.f);
	}
}

bool Snow::check()
{
	if (!db.count("radius"))
	{
		std::cerr << "variable \"radius\" not set" << std::endl;
		return false;
	}
	if (!db.count("iteration"))
	{
		std::cerr << "variable \"iteration\" not set" << std::endl;
		return false;
	}
	if (!db.count("xOrigin"))
	{
		std::cerr << "variable \"xOrigin\" not set" << std::endl;
		return false;
	}
	if (!db.count("yOrigin"))
	{
		std::cerr << "variable \"yOrigin\" not set" << std::endl;
		return false;
	}

	return true;
}

void Snow::clear()
{
	vertices.clear();
}

void Snow::generate()
{
	std::lock_guard<std::mutex> guard{mutex};

	if (!check())
		return;

	clear();

	sf::Vector2f origin;
	get("xOrigin") >> origin.x;
	get("yOrigin") >> origin.y;
	float radius;
	get("radius") >> radius;
	float angle(-pi / 2.f);
	vertices.push_back({move(origin, radius, angle), color});
	vertices.push_back({move(origin, radius, angle + pi / 3.f * 2.f), color});
	vertices.push_back({move(origin, radius, angle + pi / 3.f * 4.f), color});

	iterate(0, origin, radius, angle);
	angle += pi / 3.f;
	iterate(0, origin, radius, angle);
}

void Snow::draw(sf::RenderTarget &target)
{
	std::lock_guard<std::mutex> guard{mutex};
	target.draw(vertices.data(), vertices.size(), sf::Triangles);
}
