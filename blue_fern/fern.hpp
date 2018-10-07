#ifndef FERN_HPP
#define FERN_HPP

#include "l_system.hpp"

class Fern : public LSystem
{
	/*
	 * Require:
	 * initX (inherit)
	 * initY (inherit)
	 * color {"green", "blue"};
	 */

private:
	bool check() final
	{
		if (!db.count("color"))
		{
			std::cerr << "variable \"color\" not set" << std::endl;
			return false;
		}
		else
		{
			std::string color;
			get("color") >> color;
			if (color != "green" && color != "blue" && color != "yellow")
			{
				std::cerr << "variable \"color\" wrong value" << std::endl;
				return false;
			}
			else
			{
				return true;
			}
		}
	}

public:
	Fern() : LSystem()
	{
		set("init", "X");
		set("ruleX", "F[[-X]]F[+X]F[X]");
		set("ruleF", "FF");
		set("iteration", "10");
		set("branchRatio", "0.9999");
		set("turnAngle", "0.3");

		set("initAngle", "-0.5");
		set("initLen", "0.05");

		set("trunkShape", "line");
		set("leafShape", "line");
		set("fruitChance", "0");
	}

	void generate() final
	{
		if (!check())
			return;

		std::string color;
		get("color") >> color;
		if (color == "green")
		{
			set("rTrunk", "0");
			set("gTrunk", "190");
			set("bTrunk", "0");
			set("rLeaf", "0");
			set("gLeaf", "190");
			set("bLeaf", "0");
		}
		else if (color == "blue")
		{
			set("rTrunk", "0");
			set("gTrunk", "80");
			set("bTrunk", "255");
			set("rLeaf", "0");
			set("gLeaf", "80");
			set("bLeaf", "255");
		}
		else if (color == "yellow")
		{
			set("rTrunk", "255");
			set("gTrunk", "220");
			set("bTrunk", "0");
			set("rLeaf", "255");
			set("gLeaf", "220");
			set("bLeaf", "0");
		}
		
		LSystem::generate();
	}

	std::unique_ptr<Object> clone() final { return std::make_unique<Fern>(*this); }
};

#endif
