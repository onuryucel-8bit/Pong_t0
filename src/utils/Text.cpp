#include "Text.h"

Text::Text()
{
    m_position = { 0,0,10,10 };
}

Text::~Text()
{
    SDL_DestroyTexture(m_text);
    TTF_CloseFont(m_font);    
}

void Text::loadFont(std::string path, float fontSize)
{
    std::string fullpath = std::string(cmake_PROJECT_RES) + path;


    m_font = TTF_OpenFont(fullpath.c_str(), fontSize);

    if (!m_font)
    {
        std::cout << "Font load failed\n";
    }
}

//TODO add color etc.
void Text::setText(std::string str, SDL_Renderer* renderer)
{    
    SDL_Surface* surface = TTF_RenderText_Solid(m_font, str.c_str(), str.length(), SDL_Color{ 0,0,255,255 });

    if (!surface)
    {
        std::cout << "Surface creation failed\n";
        //return;
    }

    SDL_DestroyTexture(m_text);

    m_text = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_DestroySurface(surface);
}

void Text::setPosition(float x, float y)
{
    m_position.x = x;
    m_position.y = y;
}

void Text::setSize(float w, float h)
{
    m_position.w = w;
    m_position.h = h;
}

SDL_FRect& Text::getRect()
{
    return m_position;
}

void Text::draw(SDL_Renderer* renderer)
{       
    SDL_RenderTexture(renderer, m_text, NULL, &m_position);    
}