#ifndef CLOUD_HPP
#define CLOUD_HPP

#include "blob.hpp"

class Cloud : public Blob
{
private:
	const float light;
	const sf::Uint8 alpha;

	sf::Color getColor(const std::vector<sf::Vector2f> &shape) final;

public:
	Cloud() : Blob(),
		      light{1.f},
		      alpha{235}
	{
	}

	std::unique_ptr<Object> clone() final { return std::make_unique<Cloud>(*this); }
};

#endif
