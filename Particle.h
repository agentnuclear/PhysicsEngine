#pragma once

#include <SFML/Graphics.hpp>


class Particle {
public:

	sf::Vector2f pos;	// Current particle position
	sf::Vector2f prevPos;	//Prev particle position
	float radius;
	float dampenVal = 0.9;
	mutable sf::CircleShape shape;
	const float gravity = 500.f; //basically downward force/acceleration 

	Particle(const sf::Vector2f& startpos, float r);

	//called each frame
	void update(float dt);

	void draw(sf::RenderWindow& window) const;

};