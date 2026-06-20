#pragma once

#include <SDL3/SDL.h>

#include "Entity.h"

class Player : public Entity
{
public:
	Player();
	~Player();

	void update(float dt);
	void draw(SDL_Renderer* renderer) override;
		
	SDL_Texture* m_texture = nullptr;

	float speed = 400.0f;
	float dirx;
	float diry;
private:

};

