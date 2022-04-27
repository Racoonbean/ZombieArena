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
	const float START_SPEED = 200.f;	//����� �빮��
	const float START_HEALTH = 100.f;	//�ʱ� ���� ����
	const float START_IMMUNE_MS = 200.f;

	Vector2f position;

	std::string textureFileName;
	Sprite sprite;

	Vector2i resolustion;	//�����ػ� �����ϱ� ���� ������� (���콺 ȸ��)
	IntRect arena;	//�簢 ������ ǥ���ϴµ� ������ ǥ�õ� �׵θ� ����� �ȵ�. �� �浹 üũ�ϱ� ���� �������

	int tileSize;

	Vector2f lastDir;
	float speed;	//max speed
	float accel;

	int health;
	int maxHealth;
	float immuneMs;

	Time lastHit;	//�ǰ� ������ ���� �����ð�. �������� ���� �����ð� (���۾Ƹ� �ð������� �����ѵ�)

	const int BULLET_CACHE_SIZE = 1000;
	std::list<Bullet*> unuseBullets;
	std::list<Bullet*>useBullets;
	float distanceToMuzzle;


public:
	Player();
	~Player();

	void Shoot(Vector2f dir);

	void Spawn(IntRect arena, Vector2i res, int tileSize);
	bool OnHitted(Time timeHit);	//�ǰݴ��� ��


	Time GetLastTime() const;
	FloatRect GetGlobalBound() const;
	Vector2f GetPosition() const;
	float GetRotation() const; //ȸ��
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

