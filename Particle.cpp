#include "Particle.h"
#include <random>

Particle::Particle(const sf::Vector2f& startpos, float r)
	: pos(startpos), prevPos(startpos), radius(r), shape(r)
{
	//adding random initial velocity
	static std::mt19937 rng{ std::random_device{}() };
	std::uniform_real_distribution<float> velDist(-50.f, 50.f);
	//random color
	std::mt19937 colorrng{ std::random_device{}() };
	std::uniform_int_distribution<int> colorDist(0, 255);

	sf::Vector2f initialVelocity{ velDist(rng),velDist(rng) };
	prevPos = pos - initialVelocity; //sets starting speed

	shape.setRadius(radius);
	shape.setOrigin(sf::Vector2f(radius,radius));
	shape.setPosition(pos);
	//shape.setFillColor(sf::Color::White);
	//random colors
	shape.setFillColor(sf::Color(
		colorDist(colorrng),
		colorDist(colorrng),
		colorDist(colorrng)
	));
}

void Particle::update(float dt)
{
	//verlet solver
	sf::Vector2f velocity = pos - prevPos;	//Implicit Velocity
	prevPos = pos;							//saving current pos
	pos += velocity;						//move by velocity
	pos.y += gravity * dt * dt;				//Apply gravity.

	//collisions with window edges
	const float WIN_W = 800.f;
	const float WIN_H = 600.0f;			//can pass it in update considering resizing windows but no need rn

	if (pos.x - radius < 0.f) {
		pos.x = radius;
		prevPos.x = pos.x + velocity.x * dampenVal;	//bounce dampen
	}
	else if (pos.x + radius > WIN_W) {
		pos.x = WIN_W - radius;
		prevPos.x = pos.x + velocity.x * dampenVal;
	}
	if (pos.y - radius < 0.f) {
		pos.y = radius;
		prevPos.y = pos.y + velocity.y * dampenVal;	//bounce dampen
	}
	else if (pos.y + radius > WIN_H) {
		pos.y = WIN_H - radius;
		prevPos.y = pos.y + velocity.y * dampenVal;
	}
}

void Particle::draw(sf::RenderWindow& window) const
{
	shape.setPosition(pos);
	window.draw(shape);
}
