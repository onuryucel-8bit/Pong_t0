#include "Player.h"


Player::Player()
{
	m_body.x = 50;
	m_body.y = 50;
	m_body.w = 32;
	m_body.h = 200;
	
	dirx = 0;
	diry = 0;
	
}

Player::~Player()
{
}

void Player::update(float dt)
{
	m_body.x += dirx * dt * speed;
	m_body.y += diry * dt * speed;
}

void Player::draw(SDL_Renderer* renderer)
{
	//SDL_RenderTextureRotated(
	//	renderer,
	//	m_texture,
	//	nullptr,
	//	&m_body,
	//	25.0,          // degrees
	//	nullptr,
	//	SDL_FLIP_NONE
	//);

	SDL_SetRenderDrawColor(renderer, 0, 255, 0, 0);
	//SDL_RenderRect(renderer, &m_body);
	
	SDL_RenderTexture(renderer, m_texture, nullptr, &m_body);

	
}