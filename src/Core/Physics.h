#pragma once

#include <SDL3/SDL.h>

#include "Entity/Entity.h"

enum Wallid
{
	Left,
	Top,
	Right,
	Bottom
};

enum class CollisionType
{
	None,
	Player1,
	Player2,
	Wall_TopBottom,
	Wall_Right,
	Wall_Left
};

struct CollisionResult
{
	CollisionType m_type = CollisionType::None;
};

class Physics
{
public:
	Physics();
	~Physics();

	CollisionResult checkCollision(Entity& player1, Entity& player2, Entity& ball);
	bool collisionAABB(SDL_FRect& a, SDL_FRect& b);

	SDL_FRect m_walls[4];
private:
};

