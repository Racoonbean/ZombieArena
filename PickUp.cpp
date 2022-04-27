#include "PickUp.h"
#include "SOURCE/Utils/TextureHolder.h"
#include "SOURCE/Utils/Utils.h"

PickUp::PickUp(PickupTypes type) : type(type)
{
	std::string textureId;
	switch (this->type)
	{
	case PickupTypes::Ammo:
		textureId = "graphics/ammo_pickup.png";
		value = AMMO_START_VALUE;
		break;
	case PickupTypes::Health:
		textureId = "graphics/health_pickup.png";
		value = HEALTH_START_VALUE;
		break;S
	}
	sprite.setTexture(TextureHolder::GetTexture(textureId));
	Utils::SetOrigin(sprite, Pivots::CC);

	Spawn(false);

}

void PickUp::Update(float dt)
{
	timer -= dt;
	if (timer < 0.f)
	{
		Spawn(!spawned);
	}
}

void PickUp::Spawn(bool spawn)
{
	spawned = spawn;
	if (spawned)
	{
		timer = START_SECONDS_FOR_LIVE;

		int x = Utils::RandomRange(arena.left, arena.left + arena.width);
		int y = Utils::RandomRange(arena.top, arena.top + arena.height);

		sprite.setPosition(Vector2f(x, y));
	}
	else
	{
		timer = STAR_WAIT_TIME;
	}
}

int PickUp::GotIt()
{
	switch (type)
	{

	}
	return 0;
}

void PickUp::SetArena(IntRect rect)
{
	arena = rect;
}

bool PickUp::IsSpawned()
{
	return spawned;
}

Sprite PickUp::GetSprite()
{
	return sprite;
}

FloatRect PickUp::GetGlobalBounds()
{
	return sprite.getGlobalBounds();
}

PickupTypes PickUp::GetType()
{
	return PickupTypes();
}
