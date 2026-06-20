#include "Application.h"

Application::Application()
{    
    m_context.window = SDL_CreateWindow("Pong", m_context.WindowWidth, m_context.WindowHeight, SDL_WINDOW_RESIZABLE);

    if (m_context.window == nullptr)
    {
        std::cout << "ERROR:: couldnt init window\n";
        m_context.f_running = false;
    }

    m_context.renderer = SDL_CreateRenderer(m_context.window, NULL);

    if (m_context.renderer == nullptr)
    {
        std::cout << "ERROR:: couldnt init renderer\n"; 
        m_context.f_running = false;
    }      

    if (!TTF_Init())
    {
        std::cout << "ERROR:: TTF_init() failed\n";
        m_context.f_running = false;
    }
}

Application::~Application()
{
}

void Application::run()
{
    init();
       
    using Clock = std::chrono::steady_clock;
    using Duration = std::chrono::duration<float>;
    using TimePoint = std::chrono::steady_clock::time_point;

    TimePoint previousTime = Clock::now();
       
    while (m_context.f_running)
    {
        //------------Delta time--------------------//
        TimePoint currentTime = Clock::now();

        Duration deltaTime = currentTime - previousTime;

        previousTime = currentTime;

        float dt = deltaTime.count();
        float fps = 1.0f / dt;
        //------------------------------------------//
        
           
        StateID stateid = m_states.back()->inputs();

        switch (stateid)
        {
        case StateID::Game:
            m_states.back()->exit(); 
            m_states.push_back(&m_gameState);
            m_states.back()->enter();
            break;

        case StateID::Menu:
            m_states.back()->exit();            
            m_states.pop_back();
            m_states.back()->enter();
            break;        
        }

        m_states.back()->update(dt);
        m_states.back()->draw();
        m_states.back()->drawGui();
        
        //swap buffers
        SDL_RenderPresent(m_context.renderer);          
    }
}

void Application::init()
{                    
  
    m_gameState.init(&m_context, &m_soundManager);
    m_menuState.init(&m_context, &m_soundManager);
    
    m_states.push_back(&m_menuState);
        
 
    //ImGui init()
    //======================================//
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    io = &ImGui::GetIO();

    // Enable Docking
    io->ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io->FontGlobalScale = 1;

    ImGui_ImplSDL3_InitForSDLRenderer(m_context.window, m_context.renderer);
    ImGui_ImplSDLRenderer3_Init(m_context.renderer);
    //======================================//
   
}
