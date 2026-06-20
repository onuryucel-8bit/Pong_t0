#include "MenuState.h"

MenuState::MenuState()	
{
    
}

MenuState::~MenuState()
{
}

void MenuState::init(AppContext* appContext, SoundManager* soundManager)
{
	m_context = appContext;
    m_soundManager = soundManager;
}

void MenuState::enter()
{
    //=======================================//
    // Resize window
    //=======================================//
    //SDL_SetRenderLogicalPresentation(m_context->renderer, m_context->virtualWidth, m_context->virtualHeight, SDL_LOGICAL_PRESENTATION_STRETCH);
    //SDL_GetWindowSize(m_context->window, &m_context->WindowWidth, &m_context->WindowHeight);

    std::cout << "menuState.enter()\n";
}

void MenuState::exit()
{
    m_stateID = StateID::None;
}

void MenuState::update(float dt)
{
}

void MenuState::draw()
{
	SDL_SetRenderDrawColor(m_context->renderer, 0, 0, 0, 255);

	//clear back buffer
	SDL_RenderClear(m_context->renderer);
	
}

void MenuState::drawGui()
{
    ImGui_ImplSDLRenderer3_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();
    //===================================================//
    //===================================================//
    //===================================================//
    
    /*
    ImGui::Begin("control");
    static float width = 300;
    static float height = 350;
    static float posx = 250;
    static float posy = 100;
    
    ImGui::SliderFloat("posx", &posx, 0, 600);
    ImGui::SliderFloat("posy", &posy, 0, 600);
    ImGui::SliderFloat("width", &width, 0, 600);
    ImGui::SliderFloat("heigh", &height, 0, 600);
    ImGui::End();
    */

    //=======================================//
    // Window pos/size
    //=======================================//
    //ImGui::SetNextWindowPos(ImVec2(m_context->virtualWidth / 2 - 200, m_context->virtualHeight / 2 - 200));
    //ImGui::SetNextWindowSize(ImVec2(400, 400));

    //ImGuiIO& io = ImGui::GetIO();
    //io.DisplaySize = ImVec2(m_context->virtualWidth, m_context->virtualHeight);

    ImGuiWindowFlags flags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse;

    ImGui::Begin("menu", nullptr, flags);

    //=======================================//
    // Start button
    //=======================================//
    ImGui::SetCursorPos(ImVec2(20, 50));
    if (ImGui::Button("Start", ImVec2(360, 50)))
    {
        m_stateID = StateID::Game;
        m_soundManager->play("pause.wav");
    }

    //=======================================//
    // Exit button
    //=======================================//
    ImGui::SetCursorPos(ImVec2(20, 130));
    if (ImGui::Button("Exit", ImVec2(360, 50)))
    {
        m_context->f_running = false;
        //m_soundManager->play("pause.wav");
    }

    //=======================================//
    // Window width/height sliders
    //=======================================//
    ImGui::NewLine();
    static float tempVirtualWidth =  m_context->virtualWidth;
    static float tempVirtualHeight = m_context->virtualHeight;

    ImGui::SliderFloat("window Width", &tempVirtualWidth, 1, 1600);
    ImGui::SliderFloat("window Height", &tempVirtualHeight, 1, 800);

    if (ImGui::Button("Apply"))
    {
        m_context->virtualWidth = tempVirtualWidth;
        m_context->virtualHeight = tempVirtualHeight;
        SDL_SetWindowSize(m_context->window, m_context->virtualWidth, m_context->virtualHeight);
    }

    //=======================================//
    // Enable/disable Lua scripting
    //=======================================//
    ImGui::NewLine();
    ImGui::Text("Script is %s", m_context->f_scriptRunning ? "true" : "false");
    if (ImGui::Button("Script"))
    {
        m_context->f_scriptRunning = !m_context->f_scriptRunning;
    }

    //=======================================//
    // Mouse position
    //=======================================//
    ImGui::NewLine();
    ImVec2 mouse = ImGui::GetMousePos();
    ImGui::Text("imgui Mouse %.1f %.1f", mouse.x, mouse.y);

    ImGui::Text("keys | d: debug menu in game state\ngame state e: => menu state\nf: fullscreen");

    ImGui::End();
    //===================================================//
    //===================================================//
    //===================================================//

    ImGui::Render();
    ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), m_context->renderer);
}

StateID MenuState::inputs()
{

    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        ImGui_ImplSDL3_ProcessEvent(&event);
        
        if (event.type == SDL_EVENT_KEY_DOWN)
        {
            switch (event.key.key)
            {
            case SDLK_ESCAPE:
                m_context->f_running = false;
                break;

            case SDLK_F:
                m_context->f_fullscreen = !m_context->f_fullscreen;


                if (m_context->f_fullscreen)
                {
                    SDL_SetWindowFullscreen(m_context->window, true);
                }
                else
                {                    
                    SDL_SetWindowFullscreen(m_context->window, false);
                    SDL_SetWindowSize(m_context->window, m_context->virtualWidth, m_context->virtualHeight);
                }
                
                SDL_GetWindowSize(m_context->window, &m_context->WindowWidth, &m_context->WindowHeight);
                //SDL_SetRenderLogicalPresentation(m_context->renderer, m_context->virtualWidth, m_context->virtualHeight, SDL_LOGICAL_PRESENTATION_STRETCH);

                ImGuiIO& io = ImGui::GetIO();
                io.DisplaySize = ImVec2((float)m_context->WindowWidth, (float)m_context->WindowHeight);

                std::cout << m_context->WindowWidth << "," << m_context->WindowHeight << "\n";

                
                break;

            }
        }
        switch (event.type)
        {
        case SDL_EVENT_MOUSE_MOTION:
        {
            
            break;
        }

        case SDL_EVENT_QUIT:
            m_context->f_running = false;
            break;

        case SDL_EVENT_WINDOW_RESIZED:
        
            //m_context->WindowWidth = event.window.data1;
            //m_context->WindowHeight = event.window.data2;
            //                        
            //SDL_SetRenderLogicalPresentation(m_context->renderer, m_context->virtualWidth, m_context->virtualHeight, SDL_LOGICAL_PRESENTATION_STRETCH);
            //SDL_GetWindowSize(m_context->window, &m_context->WindowWidth, &m_context->WindowHeight);
            //ImGui_ImplSDL3_ProcessEvent(&event);


            //int w, h;
            //SDL_GetWindowSize(m_context->window, &w, &h);
           // SDL_SetWindowSize(m_context->window, w, h);

            //ImGuiIO& io = ImGui::GetIO();
            //io.DisplaySize = ImVec2((float)w, (float)h);
            break;
        
        }

    }

    return m_stateID;
}