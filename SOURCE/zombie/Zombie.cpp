#include "Zombie.h"
#include "../Utils/TextureHolder.h"
#include "../Utils/Utils.h"
#include "../player/Player.h"
#include "../Utils/InputManager.h"
#include <iostream>

std::vector<ZombieInfo> Zombie::zombieInfo;
bool Zombie::isInitInfo = false;

Zombie::Zombie() : alive(true)
{
	if (!isInitInfo)
	{
		zombieInfo.resize((int)ZombieTypes::COUNT);
		{
			auto& info = zombieInfo[(int)ZombieTypes::BLOATER];
			info.type = ZombieTypes::BLOATER;
			info.textureFileName = "graphics/bloater.png";
			info.speed = 40.f;
			info.health = 5;
		}
		
		{
			auto& info = zombieInfo[(int)ZombieTypes::CHASER];
			info.type = ZombieTypes::CHASER;
			info.textureFileName = "graphics/chaser.png";
			info.speed = 70.f;
			info.health = 1;
		}
		
		{
			auto& info = zombieInfo[(int)ZombieTypes::CRAWLER];
			info.type = ZombieTypes::CRAWLER;
			info.textureFileName = "graphics/crawler.png";
			info.speed = 20.f;
			info.health = 3;
		}
		

		isInitInfo = true;
	}
}

bool Zombie::OnHitted()
{

	//
	std::cout << "Hit" << std::endl;
	return false;
}

bool Zombie::IsAlive()
{
	return alive;
}

void Zombie::Spawn(float x, float y, ZombieTypes type)
{
	auto& info = zombieInfo[(int)type];
	sprite.setTexture(TextureHolder::GetTexture(info.textureFileName));
	speed = info.speed;
	health = info.health;

	position.x = x;
	position.y = y;

	Utils::SetOrigin(sprite, Pivots::CC);
	sprite.setPosition(position);
}

void Zombie::Update(float dt, Vector2f playerPosition)
{
	Vector2f dir = playerPosition - position;

	float length = sqrt(dir.x * dir.x + dir.y * dir.y);

	if (length > 0)
	{
		dir /= length;
	}

	if (length < speed * dt * 0.5f)	//포지션에 도착했다는 것
	{
		position = playerPosition;
	}
	else 
	{
		position += dir * speed * dt;	//distance= v * t
	}


	Vector2f replacePosition;

	replacePosition = position;
	position += dir * speed * dt;	//distance= v * t

	if (position.x<arena.left + 50 || position.x>arena.width - 50)
	{
		position.x = replacePosition.x;
	}

	if (position.y<arena.top + 50 || position.y>arena.height - 50)
	{
		position.y = replacePosition.y;
	}

	sprite.setPosition(position);

	//회전 ///////////////////////////////////////////////////////
	float radian = atan2(dir.y, dir.x);
	float degree = (radian * 180.f) / 3.141592f;

	sprite.setRotation(degree);
}

bool Zombie::UpdateCollision(Time time, Player& player)
{
	if (sprite.getGlobalBounds().intersects(player.GetGlobalBound()))
	{
		return player.OnHitted(time);
	}

	return false;
}

FloatRect Zombie::GetGlobalBound()
{
	return sprite.getGlobalBounds();
}

Sprite Zombie::GetSprite()
{
	return sprite;
}
