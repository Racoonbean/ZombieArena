#pragma once
#include "SFML/Graphics.hpp"
#include <string>
#include "../Bullet/Bullet.h"
#include <list>

using namespace sf;

class PickUp;

class Player
{
private:
	const float START_ACCEL = 200.f;
	const float START_SPEED = 200.f;	//상수는 대문자
	const float START_HEALTH = 100.f;	//초기 스탯 설정
	const float START_IMMUNE_MS = 200.f;

	Vector2f position;

	std::string textureFileName;
	Sprite sprite;

	Vector2i resolustion;	//게임해상도 설정하기 위한 멤버변수 (마우스 회전)
	IntRect arena;	//사각 영역을 표시하는데 벽돌로 표시된 테두리 벗어나면 안됨. 그 충돌 체크하기 위한 멤버변수

	int tileSize;

	Vector2f lastDir;
	float speed;	//max speed
	float accel;

	int health;
	int maxHealth;
	float immuneMs;

	Time lastHit;	//피격 당했을 때의 누적시간. 마지막에 맞은 누적시간 (슈퍼아머 시간때문에 설정한듯)

	const int BULLET_CACHE_SIZE = 1000;
	std::list<Bullet*> unuseBullets;
	std::list<Bullet*>useBullets;
	float distanceToMuzzle;


public:
	Player();
	~Player();

	void Shoot(Vector2f dir);

	void Spawn(IntRect arena, Vector2i res, int tileSize);
	bool OnHitted(Time timeHit);	//피격당할 떄


	Time GetLastTime() const;
	FloatRect GetGlobalBound() const;
	Vector2f GetPosition() const;
	float GetRotation() const; //회전
	Sprite GetSprite() const;
	int GetHealth() const;

	void Update(float dt);
	bool UpdateCollision(const std::list<PickUp*>& items);
	bool UpdateCollision(const std::vector<Zombie*>& zombies);

	void Draw(RenderWindow& window);

	void GetHealthItem(int amount);

	void UpgradeSpeed();
	void UpgradeMaxHealth();


};

