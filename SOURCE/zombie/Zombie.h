#pragma once
#include <SFML/Graphics.hpp>
#include <string>

using namespace sf;

enum class ZombieTypes
{
	BLOATER,
	CHASER,
	CRAWLER,
	COUNT
};

struct ZombieInfo
{
	ZombieTypes type;
	std::string textureFileName;
	float speed;
	int health;	//maxHealth
};
class Player;

class Zombie
{
private:
	ZombieTypes zombieType;

	Vector2f position;
	Sprite sprite;
	
	IntRect arena;

	float speed;
	int health;

	bool alive;

	static std::vector<ZombieInfo> zombieInfo;
	static bool isInitInfo;
public:
	Zombie();
	bool OnHitted();
	bool IsAlive();

	void Spawn(float x, float y, ZombieTypes type);
	void Update(float dt, Vector2f playerPosition);

	bool UpdateCollision(Time time, Player& player);

	FloatRect GetGlobalBound();
	Sprite GetSprite();

};

