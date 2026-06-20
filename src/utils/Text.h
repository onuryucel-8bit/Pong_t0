#pragma once

#include <iostream>
#include <string>

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

class Text
{
public:
	Text();
	~Text();

	void loadFont(std::string path, float fontSize);
	void draw(SDL_Renderer* renderer);

	void setText(std::string str, SDL_Renderer* renderer);
	void setPosition(float x, float y);
	void setSize(float w, float h);

	SDL_FRect& getRect();

private:
	TTF_Font* m_font = nullptr;
	SDL_Texture* m_text = nullptr;
	SDL_FRect m_position;
};

