#ifndef RED_TREE_HPP
#define RED_TREE_HPP

#include "l_system.hpp"

class RedTree : public LSystem
{
	/*
	 * Require:
	 * initX (inherit)
	 * initY (inherit)
	 * iteration (inherit)
	 */

public:
	RedTree() : LSystem()
	{
		set("branchRatio", "0.995");
		set("turnAngle", "0.1");
		set("widthRatio", "10");
		set("widthDelta", "0.975");

		set("initAngle", "-0.5");
		set("initLen", "6");
		set("init", "X");
		set("ruleX", "F[[---X]]F[+++X]F[X][++X][--X]");
		set("ruleF", "FF");

		set("trunkShape", "quad");
		set("rTrunk", "100");
		set("gTrunk", "50");
		set("bTrunk", "0");

		set("leafShape", "line");
		set("rLeaf", "200");
		set("gLeaf", "0");
		set("bLeaf", "0");

		set("fruitChance", "0.7");
		set("fruitShape", "circle");
		set("rFruit", "230");
		set("gFruit", "40");
		set("bFruit", "0");
	}

	using LSystem::generate;

	std::unique_ptr<Object> clone() final { return std::make_unique<RedTree>(*this); }
};

#endif
