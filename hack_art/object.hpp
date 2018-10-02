#ifndef OBJECT_HPP
#define OBJECT_HPP

#include <SFML/Graphics.hpp>
#include <map>
#include <string>
#include <sstream>

class Object
{
protected:
	std::map<std::string, std::string> db;
	bool canDraw;
	virtual bool check() = 0;

public:
	Object() : canDraw{false}
	{
	}
	void set(const std::string &key, const std::string &value) { db[key] = value; }
	std::istringstream get(const std::string &key) { return std::istringstream{db[key]}; }
	virtual void generate() = 0;
	virtual void draw(sf::RenderTarget &target) = 0;
};

#endif
