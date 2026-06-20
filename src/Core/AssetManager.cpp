#include "AssetManager.h"

AssetManager::AssetManager()
{
}

AssetManager::~AssetManager()
{
}

bool AssetManager::addTexture(std::string id, std::string path, SDL_Renderer* renderer)
{
	SDL_Texture* texture = IMG_LoadTexture(renderer, (std::string(cmake_PROJECT_RES) + "/" + path).c_str());

    if (!texture)
    {
        std::cout << "ERROR:: couldnt load texture: " << SDL_GetError() << "\n";        
        return true;
    }

	m_textureList[id] = texture;
    return false;
}

SDL_Texture* AssetManager::getTexture(std::string id)
{
	return m_textureList[id];
}
