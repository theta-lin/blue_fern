#ifndef L_SYSTEM_HPP
#define L_SYSTEM_HPP

#include "object.hpp"
#include "linear.hpp"
#include <stack>
#include <random>
#include <ctime>
#include <iostream>

class LSystem : public Object
{
private:

	/*
	 * Require:
	 * init
	 * ruleX
	 * ruleF
	 * iteration
	 * branchRatio
	 * turnAngle (pi)
	 * widthRatio (optional)
	 * widthDelta (optional)
	 *
	 * initX
	 * initY
	 * initAngle (pi)
	 * initLen
	 *
	 * trunkShape
	 * rTrunk
	 * gTrunk
	 * bTrunk
	 *
	 * leafShape
	 * rLeaf
	 * gLeaf
	 * bLeaf
	 *
	 * fruitChance
	 * fruitShape
	 * rFruit
	 * gFruit
	 * bFruit
	 */

	/*
	 * X no action, for evolving
	 * F forward
	 * - turn counter-clockwise
	 * + turn clockwise
	 * [ push current onto stack
	 * ] pop stack onto current
	 * T trunk forward
	 */

	/*
	 * Shapes:
	 * line
	 * quad
	 * circle
	 */

	std::vector<sf::Vertex> lines;
	std::vector<sf::Vertex> quads;
	std::vector<sf::CircleShape> circles;

	void replaceChar(std::string &str, char search, char replace);
	void dump(const Vector2D &segment, char symbol, float width);

	bool check() override;
	void clear() final;

public:
	LSystem() : Object()
	{
	}

	void generate() override;
	void draw(sf::RenderTarget &target) final;
	std::unique_ptr<Object> clone() override { return std::make_unique<LSystem>(*this); }
};

#endif
