#ifndef OBJECT_HPP
#define OBJECT_HPP

#include <SFML/Graphics.hpp>
#include <map>
#include <string>
#include <sstream>
#include <mutex>
#include <memory>

class Object
{
protected:
	std::map<std::string, std::string> db;
	std::mutex mutex;
	bool canUpdate;
	bool canDraw;
	virtual bool check() = 0;
	virtual void clear() = 0;

public:
	Object() : canUpdate{false}, canDraw{false}
	{
	}

	Object(const Object &object) : db{object.db}, canDraw{false}
	{
	}

	void set(const std::string &key, const std::string &value) { db[key] = value; }
	std::istringstream get(const std::string &key) { return std::istringstream{db[key]}; }

	virtual void update()
	{
	}
	virtual void generate() = 0;
	virtual void draw(sf::RenderTarget &target) = 0;
	virtual std::unique_ptr<Object> clone() = 0;
};

#endif
