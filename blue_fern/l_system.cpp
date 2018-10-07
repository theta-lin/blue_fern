#include "l_system.hpp"

void LSystem::replaceChar(std::string &str, char search, char replace)
{
	for (auto &symbol : str)
	{
		if (symbol == search)
			symbol = replace;
	}
}

void LSystem::dump(const Vector2D &segment, char symbol, float width)
{
	std::string shape;
	int r, g, b;
	static std::mt19937 generator{static_cast<unsigned int>(time(0))};
	static std::uniform_real_distribution<float> dist{0.f, 1.f};

	switch (symbol)
	{
	case 'T':
		get("trunkShape") >> shape;
		get("rTrunk") >> r;
		get("gTrunk") >> g;
		get("bTrunk") >> b;
		break;

	case 'F':
	{
		float fruitChance;
		get("fruitChance") >> fruitChance;

		if (dist(generator) < fruitChance)
		{
			get("fruitShape") >> shape;
			get("rFruit") >> r;
			get("gFruit") >> g;
			get("bFruit") >> b;
		}
		else
		{
			get("leafShape") >> shape;
			get("rLeaf") >> r;
			get("gLeaf") >> g;
			get("bLeaf") >> b;
		}
		break;
	}
	}
	sf::Color color{static_cast<sf::Uint8>(r), static_cast<sf::Uint8>(g), static_cast<sf::Uint8>(b)};

	if (shape == "line")
	{
		lines.push_back({segment.p1, color});
		lines.push_back({segment.p2, color});
	}
	else if (shape == "quad")
	{
		float angle{std::atan2(segment.p1.y - segment.p2.y, segment.p1.x - segment.p2.x)};
		float angle1{angle - pi / 2.f};
		float angle2{angle + pi / 2.f};
		float dist{width / 2.f};
		sf::Vector2f a{segment.p1}, b{segment.p1};
		sf::Vector2f c{segment.p2}, d{segment.p2};
		a = move(a, dist, angle1);
		b = move(b, dist, angle2);
		c = move(c, dist, angle2);
		d = move(d, dist, angle1);
		quads.push_back({a, color});
		quads.push_back({b, color});
		quads.push_back({c, color});
		quads.push_back({d, color});
	}
	else if (shape == "circle")
	{
		float radius{getMag(segment) / 2};
		sf::Vector2f center{(segment.p1.x + segment.p2.x) / 2, (segment.p1.y + segment.p2.y) / 2};
		sf::CircleShape circle{radius};
		circle.setOrigin(radius, radius);
		circle.setPosition(center);
		circle.setFillColor(color);
		circles.push_back(circle);
	}
}

bool LSystem::check()
{
	if (!db.count("init"))
	{
		std::cerr << "variable \"init\" not set" << std::endl;
		return false;
	}
	if (!db.count("ruleX"))
	{
		std::cerr << "variable \"ruleX\" not set" << std::endl;
		return false;
	}
	if (!db.count("ruleF"))
	{
		std::cerr << "variable \"ruleF\" not set" << std::endl;
		return false;
	}
	if (!db.count("iteration"))
	{
		std::cerr << "variable \"iteration\" not set" << std::endl;
		return false;
	}
	if (!db.count("branchRatio"))
	{
		std::cerr << "variable \"branchRatio\" not set" << std::endl;
		return false;
	}
	if (!db.count("turnAngle"))
	{
		std::cerr << "variable \"turnAngle\" not set" << std::endl;
		return false;
	}

	if (!db.count("initX"))
	{
		std::cerr << "variable \"initX\" not set" << std::endl;
		return false;
	}
	if (!db.count("initY"))
	{
		std::cerr << "variable \"initY\" not set" << std::endl;
		return false;
	}
	if (!db.count("initAngle"))
	{
		std::cerr << "variable \"initAngle\" not set" << std::endl;
		return false;
	}
	if (!db.count("initLen"))
	{
		std::cerr << "variable \"initLen\" not set" << std::endl;
		return false;
	}
	
	if (!db.count("trunkShape"))
	{
		std::cerr << "variable \"trunkShape\" not set" << std::endl;
		return false;
	}
	if (!db.count("rTrunk"))
	{
		std::cerr << "variable \"rTrunk\" not set" << std::endl;
		return false;
	}
	if (!db.count("gTrunk"))
	{
		std::cerr << "variable \"gTrunk\" not set" << std::endl;
		return false;
	}
	if (!db.count("bTrunk"))
	{
		std::cerr << "variable \"bTrunk\" not set" << std::endl;
		return false;
	}

	if (!db.count("leafShape"))
	{
		std::cerr << "variable \"leafShape\" not set" << std::endl;
		return false;
	}
	if (!db.count("rLeaf"))
	{
		std::cerr << "variable \"rLeaf\" not set" << std::endl;
		return false;
	}
	if (!db.count("gLeaf"))
	{
		std::cerr << "variable \"gLeaf\" not set" << std::endl;
		return false;
	}
	if (!db.count("bLeaf"))
	{
		std::cerr << "variable \"bLeaf\" not set" << std::endl;
		return false;
	}

	if (!db.count("fruitChance"))
	{
		std::cerr << "variable \"fruitChance\" not set" << std::endl;
		return false;
	}
	else
	{
		float fruitChance;
		get("fruitChance") >> fruitChance;
		if (fruitChance > 0)
		{
			if (!db.count("fruitShape"))
			{
				std::cerr << "variable \"fruitShape\" not set" << std::endl;
				return false;
			}
			if (!db.count("rFruit"))
			{
				std::cerr << "variable \"rFruit\" not set" << std::endl;
				return false;
			}
			if (!db.count("gFruit"))
			{
				std::cerr << "variable \"gFruit\" not set" << std::endl;
				return false;
			}
			if (!db.count("bFruit"))
			{
				std::cerr << "variable \"bFruit\" not set" << std::endl;
				return false;
			}
		}
	}
		
	return true;
}

void LSystem::clear()
{
	lines.clear();
	quads.clear();
	circles.clear();
}

void LSystem::generate()
{
	std::lock_guard<std::mutex> guard{mutex};
	if (!LSystem::check())
		return;

	clear();

	std::string pattern;
	get("init") >> pattern;
	std::string ruleX;
	get("ruleX") >> ruleX;
	std::string ruleF;
	get("ruleF") >> ruleF;

	std::string ruleT{ruleF};
	replaceChar(ruleT, 'F', 'T');

	int iteration;
	get("iteration") >> iteration;
	for (int i{0}; i < iteration; ++i)
	{
		std::string next;
		for (auto &symbol : pattern)
		{
			switch (symbol)
			{
			case 'X':
				next += ruleX;
				break;

			case 'F':
			{
				if (i < iteration - 1)
					next += ruleF;
				else
					next += ruleT;
				break;
			}

			default:
				next += symbol;
				break;
			}
		}

		pattern = next;
	}

	std::stack<sf::Vector2f> posStack;
	std::stack<float> angleStack;
	std::stack<float> lenStack;
	std::stack<float> widthStack;

	sf::Vector2f pos;
	get("initX") >> pos.x;
	get("initY") >> pos.y;
	float angle;
	get("initAngle") >> angle;
	angle *= pi;
	float len;
	get("initLen") >> len;
	float width;
	get("widthRatio") >> width;
	width *= len;

	float branchRatio;
	get("branchRatio") >> branchRatio;
	float turnAngle;
	get("turnAngle") >> turnAngle;
	turnAngle *= pi;
	float widthDelta;
	get("widthDelta") >> widthDelta;

	for (auto &symbol : pattern)
	{
		switch (symbol)
		{
		case 'F':
		{
			sf::Vector2f oldPos{pos};
			pos = move(pos, len, angle);
			dump({oldPos, pos}, 'F', width);
			len *= branchRatio;
			width *= widthDelta;
			break;
		}

		case 'T':
		{
			sf::Vector2f oldPos{pos};
			pos = move(pos, len, angle);
			dump({oldPos, pos}, 'T', width);
			len *= branchRatio;
			width *= widthDelta;
			break;
		}

		case '+':
			angle += turnAngle;
			len *= branchRatio;
			width *= widthDelta;
			break;

		case '-':
			angle -= turnAngle;
			len *= branchRatio;
			width *= widthDelta;
			break;

		case '[':
			posStack.push(pos);
			angleStack.push(angle);
			lenStack.push(len);
			widthStack.push(width);
			break;

		case ']':
			pos = posStack.top();
			posStack.pop();
			angle = angleStack.top();
			angleStack.pop();
			len = lenStack.top();
			lenStack.pop();
			width = widthStack.top();
			widthStack.pop();
			break;

		default:
			break;
		}
	}

	canDraw = true;
}

void LSystem::draw(sf::RenderTarget &target)
{
	std::lock_guard<std::mutex> guard{mutex};
	if (canDraw)
	{
		target.draw(lines.data(), lines.size(), sf::Lines);
		target.draw(quads.data(), quads.size(), sf::Quads);
		for (auto &circle : circles)
			target.draw(circle);
	}
}
