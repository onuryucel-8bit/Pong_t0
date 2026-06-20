#include "Ball.h"

Ball::Ball()
{ 
	m_body.x = 100;
	m_body.y = 300;
	m_body.w = 32;
	m_body.h = 32;
}

Ball::~Ball()
{
}

void Ball::reset(float x, float y)
{
	/*
    if(rand() % 2 == 1)
    {
        1
    }
    else(0)
    {
        -1
    }
    */
	//dirx = (rand() % 2) ? 1 : -1;
	//diry = (rand() % 2) ? 1 : -1;

	dirx = -1;
	diry = 0;

	m_body.x = x;
	m_body.y = y;
}

void Ball::update(float dt)
{
	m_body.x += dirx * dt * speedX;
	m_body.y += diry * dt * speedY;
}

void Ball::draw(SDL_Renderer* renderer)
{
	//SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
	//SDL_RenderFillRect(renderer, &m_body);
	
	SDL_RenderTexture(renderer, m_texture, nullptr, &m_body);
}