#ifndef TREE_HPP
#define TREE_HPP

#include "l_system.hpp"

class Tree : public LSystem
{
	/*
	 * Require:
	 * initX (inherit)
	 * initY (inherit)
	 * init  (inherit / optional)
	 * ruleX (inherit / optional)
	 * ruleF (inherit / optional)
	 * variation (optional)
	 */

public:
	Tree() : LSystem()
	{
		set("iteration", "5");
		set("branchRatio", "0.99");
		set("turnAngle", "0.2");
		set("widthRatio", "10");
		set("widthDelta", "0.96");

		set("initAngle", "-0.5");
		set("initLen", "4");

		set("trunkShape", "quad");
		set("rTrunk", "100");
		set("gTrunk", "50");
		set("bTrunk", "0");

		set("leafShape", "circle");
		set("rLeaf", "0");
		set("gLeaf", "190");
		set("bLeaf", "0");

		set("fruitChance", "0");
	}

	void generate() final
	{
		if (db.count("variation"))
		{
			int variation;
			get("variation") >> variation;
			switch (variation)
			{
			case 0:
				set("init", "X");
				set("ruleX", "F[--FX][-FX][FX][+FX][++FX]");
				set("ruleF", "FF");
				break;

			case 1:
				set("init", "X");
				set("ruleX", "F[FX]F[-FX][+FX]F[++FX]");
				set("ruleF", "FF");
				break;

			case 2:
				set("init", "X");
				set("ruleX", "F[-FX][--FX]+F[FX]--F[X]");
				set("ruleF", "FF");
				break;

			case 3:
				set("init", "X");
				set("ruleX", "F[-X][++FX]--FFX++FFX");
				set("ruleF", "FF");
				break;

			case 4:
				set("init", "X");
				set("ruleX", "F[--FX][+FXX]--FFX++FX");
				set("ruleF", "FF");
				break;

			case 5:
				set("init", "X");
				set("ruleX", "--[FX]++++[FX]--[FX]");
				set("ruleF", "FF[FX]");
				break;

			default:
				break;
			}
		}

		LSystem::generate();
	}

	std::unique_ptr<Object> clone() override { return std::make_unique<Tree>(*this); }
};

#endif
