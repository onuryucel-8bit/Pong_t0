#pragma once

#include <iostream>
#include <string>
#include <unordered_map>

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

class AssetManager
{
public:
	AssetManager();
	~AssetManager();

	bool addTexture(std::string id, std::string path, SDL_Renderer* renderer);
	SDL_Texture* getTexture(std::string id);


private:
	//TODO destroy SDL_texture?
	std::unordered_map<std::string, SDL_Texture*> m_textureList;
};

