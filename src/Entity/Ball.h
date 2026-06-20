#pragma once

#include <SDL3/SDL.h>
#include <stdlib.h>

#include "Entity.h"

class Ball : public Entity
{
public:
	Ball();
	~Ball();

	void update(float dt);
	void draw(SDL_Renderer* renderer) override;
	void reset(float x, float y);
	
	SDL_Texture* m_texture;	

	float speedX = 300.0f;
	float speedY = 300.0f;
	float dirx = 0;
	float diry = 0;
private:

};

