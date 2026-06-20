#pragma once

#include <iostream>
#include <SDL3/SDL.h>

struct AppContext
{         
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;  
    bool f_running = true;
    bool f_fullscreen = false;
    bool f_scriptRunning = false;

    int WindowWidth = 800;
    int WindowHeight = 600;
   
    float virtualWidth = 800.0f;
    float virtualHeight = 600.0f;
    
    float scaledWidth = (float)WindowWidth / virtualWidth;
    float scaledHeight = (float)WindowHeight / virtualHeight;
};