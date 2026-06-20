#pragma once

#include <SDL3/SDL.h>

class Entity
{
public:
	Entity();
	~Entity();

	virtual void draw(SDL_Renderer* renderer) = 0;

	SDL_FRect m_body;
private:

};

