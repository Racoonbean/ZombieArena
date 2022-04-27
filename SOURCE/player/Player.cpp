#include "Player.h"
#include "../Utils/Utils.h"
#include "../Utils/InputManager.h"
#include <cmath>
#include "../Utils/TextureHolder.h"
#include "../Utils/PickUp.h"
#include <iostream>
#include <algorithm>

Player::Player()
	:speed(START_SPEED), health(START_HEALTH), maxHealth(START_HEALTH), immuneMs(START_IMMUNE_MS), accel(START_ACCEL),
	arena(), resolustion(), tileSize(0.f),textureFileName("graphics/player.png")
{
	distanceToMuzzle = 45.f;
	sprite.setTexture(TextureHolder::GetTexture(textureFileName));
	Utils::SetOrigin(sprite, Pivots::CC);

	for (int i = 0; i < BULLET_CACHE_SIZE; ++i)
	{
		unuseBullets.push_back(new Bullet());
	}
}

Player::~Player()
{
	for (auto bullet : unuseBullets)
	{
		delete bullet;
	}
	unuseBullets.clear();
	for (auto bullet : useBullets)
	{
		delete bullet;
	}
	useBullets.clear();
}

void Player::Shoot(Vector2f dir)
{
	// 가져오고 초기화 발사 까지
	dir = Utils::Normalize(dir);

	Vector2f spawnPos = position + dir * distanceToMuzzle;

	if (unuseBullets.empty())
	{
		for (int i = 0; i < BULLET_CACHE_SIZE; ++i)
		{
			unuseBullets.push_back(new Bullet());
		}
	}
	Bullet* bullet = unuseBullets.front();
	unuseBullets.pop_front();
	useBullets.push_back(bullet);
	bullet->Shoot(spawnPos, dir);
}

void Player::Spawn(IntRect arena, Vector2i res, int tileSize)
{
	this->arena = arena;
	resolustion = res;
	this->tileSize = tileSize;

	position.x = this->arena.width * 0.5f;
	position.y = this->arena.height * 0.5f;
}

bool Player::OnHitted(Time timeHit)
{
	if (lastHit.asMilliseconds() - lastHit.asMilliseconds() > immuneMs)
	{
		std::cout << timeHit.asSeconds() << std::endl;
		lastHit = timeHit;
		health -= 10;
		return true;
	}
	return false;
}


Time Player::GetLastTime() const
{
	return lastHit;
}

FloatRect Player::GetGlobalBound() const
{
	return sprite.getGlobalBounds();
}

Vector2f Player::GetPosition() const
{
	return position;
}

float Player::GetRotation() const
{
	return sprite.getRotation();
}

Sprite Player::GetSprite() const
{
	return sprite;
}

int Player::GetHealth() const
{
	return health;
}

void Player::Update(float dt)
{
	// 사용자입력 ///////////////////////////////////////////////////////////////////////////
	float h = InputManager::GetAxis(Axis::Horizontal);
	float v = InputManager::GetAxis(Axis::Vertical);
	Vector2f dir(h, v);
	float length = sqrt(dir.x * dir.x + dir.y * dir.y);

	if (length > 1)
	{
		dir /= length;
	}

	//이동 //////////////////////////////////////////////////////////////////////////////////
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

	//회전 /////////////////////////////////////////////////////////////////////////////////
	Vector2i mousePos = InputManager::GetMousePosition();
	Vector2i mouseDir;
	mouseDir.x = mousePos.x - resolustion.x * 0.5f;
	mouseDir.y = mousePos.y - resolustion.y * 0.5f;
	
	//mouseDir.x = mousePos.x - position.x;
	//mouseDir.y = mousePos.y - position.y;

	float radian = atan2(mouseDir.y, mouseDir.x);
	float degree = (radian * 180.f) / 3.141592f;

	sprite.setRotation(degree);

	//발사체 /////////////////////////////////////////////////////////////////////////////

	if (InputManager::GetMouseButtonDown(Mouse::Button::Left))
	{
		Shoot(Vector2f (mouseDir.x, mouseDir.y));
	}

	auto it = useBullets.begin();
	while (it != useBullets.end())
	{
		Bullet* bullet = *it;
		bullet->Update(dt);

		if (!bullet->IsActive())
		{
			it = useBullets.erase(it);
			unuseBullets.push_back(bullet);
		}
		else
		{
			++it;
		}
	}

}

bool Player::UpdateCollision(const std::list<PickUp*>& items)
{
	FloatRect bounds = sprite.getGlobalBounds();
	bool isCollided = false;
	for (auto item : items)
	{

		if (bounds.intersects(item->GetGlobalBounds()))
		{
			item->GotIt();
			isCollided = true;
		};
	}
	return isCollided;
}

bool Player::UpdateCollision(const std::vector<Zombie*>& zombies)
{
	bool isCollided = false;
	for (auto bullet : useBullets)
	{
		if (bullet->UpdateCollision(zombies))
		{
			isCollided = true;
		}
	}
	return isCollided;
}

void Player::Draw(RenderWindow& window)
{
	window.draw(sprite);
	for (auto bullet : useBullets)
	{
		window.draw(bullet->GetShape());
	}

}

void Player::GetHealthItem(int amount)
{
	health += amount;
	if (health > maxHealth)
	{
		health = maxHealth;
	}
	if (health < 0)
	{
		health = 0;
	}
}

void Player::UpgradeSpeed()
{
	speed += START_SPEED * 0.2f;
}

void Player::UpgradeMaxHealth()
{
	maxHealth += START_HEALTH * 0.2f;
}
