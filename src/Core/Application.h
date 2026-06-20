#pragma once

#include <iostream>
#include <chrono>
#include <vector>

#include <SDL3/SDL.h>


//#include <SDL3_ttf/SDL_ttf.h>

#include "backends/imgui_impl_sdl3.h"
#include "backends/imgui_impl_sdlrenderer3.h"

#include "AppContext.h"

#include "States/GameState.h"
#include "States/MenuState.h"

#include "SoundManager.h"

class Application
{
public:
    Application();
    ~Application();
    void run();
  
    bool f_running = true;

private:      
    void init();
    
    AppContext m_context;
    SoundManager m_soundManager;
    
    GameState m_gameState;
    MenuState m_menuState;    
        
    std::vector<State*> m_states;
    
    ImGuiIO* io = nullptr;
    
   
    
   

    float mouseX = 0, mouseY = 0;
    
    
};