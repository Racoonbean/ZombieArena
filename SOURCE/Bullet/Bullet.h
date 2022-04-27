#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;

class Zombie;

class Bullet
{
private:
	const float DEFAULT_SPEED = 200.f;
	const float DEFAULT_DISTANCE = 1500.f;

	bool isActive;

	RectangleShape shape;

	Vector2f position;
	Vector2f direction;

	float speed;
	float distance;

public:
	Bullet();

	void SetActive(bool active);
	void Shoot(Vector2f pos, Vector2f dir);
	void Stop();
	void Update(float dt);

	bool UpdateCollision(const std::vector<Zombie*>& zombies);

	bool IsActive();
	RectangleShape GetShape();


};

