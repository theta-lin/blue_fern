#ifndef ICE_HPP
#define ICE_HPP

#include "blob.hpp"

class Ice : public Blob
{
private:
	const float green;
	const float blue;
	const sf::Uint8 alpha;

	sf::Color getColor(const std::vector<sf::Vector2f> &shape) final;

public:
	Ice() : Blob(),
		green{0.8f},
		blue{1.f},
		alpha{180}
	{
	}

	std::unique_ptr<Object> clone() final { return std::make_unique<Ice>(*this); }
};

#endif
