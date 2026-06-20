#include "GameState.h"

GameState::GameState()    
{
    
}

void GameState::init(AppContext* appContext, SoundManager* soundManager)
{
    ls = luaL_newstate();
    luaL_openlibs(ls);
    if (luaL_dofile(ls, cmake_PROJECT_SCRIPT "main.lua") != LUA_OK)
    {
        std::cout << "Lua Error: " << lua_tostring(ls, -1) << '\n';
    }
    lua_pushlightuserdata(ls, this);
    lua_setglobal(ls, "__game");

    lua_pushcfunction(ls, lu_setPlayer1Pos);
    lua_setglobal(ls, "lu_setPlayer1Pos");

    lua_pushcfunction(ls, lu_setPlayer2Pos);
    lua_setglobal(ls, "lu_setPlayer2Pos");

    lua_pushcfunction(ls, lu_setBallPos);
    lua_setglobal(ls, "lu_setBallPos");


    m_context = appContext;
    m_soundManager = soundManager;

    srand(time(NULL));

    //=======================================//
    // Resize window
    //=======================================//
    m_scaleX = (float)m_context->WindowWidth / m_context->virtualWidth;
    m_scaleY = (float)m_context->WindowHeight / m_context->virtualHeight;

    m_context->scaledWidth = m_context->virtualWidth * m_scaleX;
    m_context->scaledHeight = m_context->scaledHeight * m_scaleY;

    //=======================================//
    //set position of ball and player
    //=======================================//
    player1.m_body.x = 50 * m_scaleX;
    player1.m_body.y = m_context->virtualHeight / 2 * m_scaleY;

    player2.m_body.x = (m_context->virtualWidth - 50) * m_scaleX;
    player2.m_body.y = m_context->virtualHeight / 2 * m_scaleY;

    ball.reset(m_context->virtualWidth / 2 * m_scaleX, m_context->virtualHeight / 2 * m_scaleY);

    //=======================================//
    //Load texture
    //=======================================//

    
    bool err = m_assetManager.addTexture("ball", "texture/ball.png", m_context->renderer);
    err |= m_assetManager.addTexture("player", "texture/paddle.png", m_context->renderer);
  
    if (err == true)
    {
        m_context->f_running = false;
        return;
    }

    ball.m_texture = m_assetManager.getTexture("ball");
    player1.m_texture = m_assetManager.getTexture("player");
    player2.m_texture = m_assetManager.getTexture("player");
      
    //=======================================//
    // Player_1 texture
    //=======================================//
    player1.m_body.w = player1.m_texture->w * m_scaleX;
    player1.m_body.h = player1.m_texture->h * m_scaleY;

    //=======================================//
    // Player_2 texture
    //=======================================//
    player2.m_body.w = player2.m_texture->w * m_scaleX;
    player2.m_body.h = player2.m_texture->h * m_scaleY;

    //=======================================//
    // ball texture 
    //=======================================//
    ball.m_body.w = ball.m_texture->w * m_scaleX;
    ball.m_body.h = ball.m_texture->h * m_scaleY;

    //=======================================//
    //create texts
    //=======================================//
    m_pausedText.loadFont("fonts/Super Mario Bros. NES.ttf", 32);


    //=======================================//
    // Paused text
    //=======================================//
    m_pausedText.setText("Paused!", m_context->renderer);
    m_pausedText.setPosition(m_context->virtualWidth / 2 - 150, m_context->virtualHeight / 2 - 100);
    m_pausedText.setSize(300, 200);

    //=======================================//
    // Player1 score text
    //=======================================//
    m_player1ScoreText.loadFont("fonts/Super Mario Bros. NES.ttf", 32);
    m_player1ScoreText.setText("0", m_context->renderer);
    m_player1ScoreText.setPosition(10 * m_scaleX, 10 * m_scaleY);
    m_player1ScoreText.setSize(50, 50);

    //=======================================//
    // Player2 score text
    //=======================================//
    m_player2ScoreText.loadFont("fonts/Super Mario Bros. NES.ttf", 32);
    m_player2ScoreText.setText("0", m_context->renderer);
    m_player2ScoreText.setPosition((m_context->virtualWidth - 70) * m_scaleX, 10 * m_scaleY);
    m_player2ScoreText.setSize(50, 50);
        
    //Walls
    phy.m_walls[Wallid::Left] = 
    { 
        0, -1, 
        10, (float)m_context->WindowHeight 
    };

    phy.m_walls[Wallid::Top]   = 
    {
        0, -1, 
        (float)m_context->WindowWidth, 1
    };

    phy.m_walls[Wallid::Right] = 
    { 
        (float)m_context->WindowWidth - 5, 0, 
        10, (float)m_context->WindowHeight 
    };

    phy.m_walls[Wallid::Bottom] = 
    { 
        0, (float)m_context->WindowHeight, 
        (float)m_context->WindowWidth, 0 
    };


    m_entityList.push_back(&player1);
    m_entityList.push_back(&player2);
    m_entityList.push_back(&ball);
}

void GameState::enter()
{
    //=======================================//
    // Lua reload main.lua
    //=======================================//
    if (luaL_dofile(ls, cmake_PROJECT_SCRIPT "main.lua") != LUA_OK)
    {
        std::cout << "Lua Error: " << lua_tostring(ls, -1) << '\n';
    }

    //=======================================//
    // Resize window
    //=======================================//
    m_scaleX = (float)m_context->WindowWidth / m_context->virtualWidth;
    m_scaleY = (float)m_context->WindowHeight / m_context->virtualHeight;
    
    m_context->scaledWidth = m_context->virtualWidth * m_scaleX;
    m_context->scaledHeight = m_context->scaledHeight * m_scaleY;

    //SDL_SetRenderLogicalPresentation(m_context->renderer, m_context->virtualWidth, m_context->virtualHeight, SDL_LOGICAL_PRESENTATION_STRETCH);
    SDL_GetWindowSize(m_context->window, &m_context->WindowWidth, &m_context->WindowHeight);
    
    //ImGuiIO& io = ImGui::GetIO();
    //io.DisplaySize = ImVec2((float)m_context->WindowWidth, (float)m_context->WindowHeight);

    //=======================================//
    // Ball pos reset
    //=======================================//
    ball.reset(m_context->virtualWidth / 2 * m_scaleX, m_context->virtualHeight / 2 * m_scaleY);

    initScene();

    f_paused = false;    
}

void GameState::exit()
{
    m_stateID = StateID::None;
}

void GameState::update(float dt)
{
    //if game paused do nothing return back to game loop
    if (f_paused)
    {
        return;
    }

    if (m_context->f_scriptRunning)
    {
        lua_getglobal(ls, "update");
        if (lua_isfunction(ls, -1))
        {
            lua_pushnumber(ls, dt);
            lua_pcall(ls, 1, 0, 0);
        }
        return;
    }
            
    //=======================================//
    // Update players and ball
    //=======================================//
    player1.update(dt);
    player2.update(dt);
            
    ball.update(dt);

    
    
    
    //=======================================//
    // Collision check 
    //=======================================//
    clampPlayer(player1);
    clampPlayer(player2);


    CollisionResult collisionResult  = phy.checkCollision(player1, player2, ball);

    switch (collisionResult.m_type)
    {
    case CollisionType::Player1:
        collisionPlayerBall(player1, ball);
        break;

    case CollisionType::Player2:
        collisionPlayerBall(player2, ball);
        break;

    case CollisionType::Wall_TopBottom:
        ball.diry *= -1;
        m_soundManager->play("game_clik.wav");
        break;

    case CollisionType::Wall_Left:
        player2Score++;
        m_player2ScoreText.setText(std::to_string(player2Score), m_context->renderer);

        m_soundManager->play("jump_08.wav");

        ball.reset(m_context->virtualWidth / 2 * m_scaleX, m_context->virtualHeight / 2 * m_scaleY);
        break;

    case CollisionType::Wall_Right:
        player1Score++;
        m_player1ScoreText.setText(std::to_string(player1Score), m_context->renderer);

        m_soundManager->play("jump_08.wav");

        ball.reset(m_context->virtualWidth / 2 * m_scaleX, m_context->virtualHeight / 2 * m_scaleY);
        break;
    }
}

void GameState::draw()
{
    //clear back buffer
    SDL_SetRenderDrawColor(m_context->renderer, 0, 0, 0, 255);    
    SDL_RenderClear(m_context->renderer);

    //=======================================//
    // Draw Score texts
    //=======================================//
    m_player1ScoreText.draw(m_context->renderer);
    m_player2ScoreText.draw(m_context->renderer);

    //=======================================//
    // Draw player and ball
    //=======================================//

    for (const auto& e : m_entityList)
    {
        e->draw(m_context->renderer);
    }
    
    //=======================================//
     
    //vertical
    SDL_RenderLine(m_context->renderer, m_context->virtualWidth / 2 * m_scaleX, 0, m_context->virtualWidth / 2 * m_scaleX, m_context->virtualHeight * m_scaleY);

    //horizontal
    SDL_RenderLine(m_context->renderer, 0, m_context->virtualHeight / 2 * m_scaleY, m_context->virtualWidth * m_scaleX, m_context->virtualHeight /2 * m_scaleY);

    SDL_RenderFillRect(m_context->renderer, &phy.m_walls[Wallid::Left]);
    SDL_RenderFillRect(m_context->renderer, &phy.m_walls[Wallid::Top]);
    SDL_RenderFillRect(m_context->renderer, &phy.m_walls[Wallid::Right]);
    SDL_RenderFillRect(m_context->renderer, &phy.m_walls[Wallid::Bottom]);

    //=======================================//
    //if game is paused
    if (f_paused)
    {
        m_pausedText.draw(m_context->renderer);
    }
    
}

void GameState::drawGui()
{
    //if 'D' key pressed draw debug gui
    if (!f_debug)
    {
        return;
    }
    
    ImGui_ImplSDLRenderer3_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();
    //===================================================//
    //===================================================//
    //===================================================//
   
    ImGui::Begin("Ball Menu");

    ImGui::SliderFloat("ball.x", &ball.m_body.x, 0, m_context->virtualWidth - ball.m_body.w);
    ImGui::SliderFloat("ball.y", &ball.m_body.y, 0, m_context->virtualHeight - ball.m_body.h);

    if (ImGui::Button("Left"))
    {
        ball.dirx = -1;
        m_soundManager->play("pause.wav");
    }

    ImGui::SameLine();
    if (ImGui::Button("Right"))
    {
        ball.dirx = 1;
        m_soundManager->play("pause.wav");
    }

    if (ImGui::Button("Up"))
    {
        ball.diry = -1;
        m_soundManager->play("pause.wav");
    }

    ImGui::SameLine();
    if (ImGui::Button("Down"))
    {
        ball.diry = 1;
        m_soundManager->play("pause.wav");
    }
    
    ImGui::SliderFloat("ball.speedx", &ball.speedX, 0, 700.0f);
    ImGui::SliderFloat("ball.speedy", &ball.speedY, 0, 700.0f);
    
    
    if (ImGui::Button("Ball.reset()"))
    {
        ball.reset(m_context->virtualWidth / 2, m_context->virtualHeight / 2);
        m_soundManager->play("pause.wav");
    }

    ImGui::End();
    //===================================================//
    //===================================================//
    ImGui::Begin("Player");
    ImGui::SliderFloat("player1.x", &player1.m_body.x, 0, m_context->virtualWidth - player1.m_body.w);
    ImGui::SliderFloat("player1.y", &player1.m_body.y, 0, m_context->virtualHeight - player1.m_body.h);
    ImGui::SliderFloat("player1.speed", &player1.speed, 0, 800.0f);

    ImGui::NewLine();
    ImGui::SliderFloat("player2.x", &player2.m_body.x, 0, m_context->virtualWidth - player2.m_body.w);
    ImGui::SliderFloat("player2.y", &player2.m_body.y, 0, m_context->virtualHeight - player2.m_body.h);
    ImGui::SliderFloat("player2.speed", &player2.speed, 0, 800.0f);
    ImGui::End();
    //===================================================//
    //===================================================//
    ImGui::Begin("State Menu");

    if (ImGui::Button("menu->"))
    {
        m_stateID = StateID::Menu;
        m_soundManager->play("pause.wav");
    }

    if (ImGui::Button("pause->"))
    {
        f_paused = !f_paused;        
        m_soundManager->play("pause.wav");
    }

    ImGui::Text("Player1 : %i", player1Score);
    ImGui::Text("Player2 : %i", player2Score);

    ImGui::Text("mouseX %.0f, mouseY %.0f", mouseX, mouseY);
    
    ImGui::End();

    
    //===================================================//
    //===================================================//
    //===================================================//

    ImGui::Render();
    ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), m_context->renderer);
    
}

StateID GameState::inputs()
{    
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        ImGui_ImplSDL3_ProcessEvent(&event);
        switch (event.type)
        {
        case SDL_EVENT_QUIT:
            m_context->f_running = false;
            break;

        case SDL_EVENT_WINDOW_RESIZED:

            //SDL_SetRenderLogicalPresentation(m_context->renderer, m_context->virtualWidth, m_context->virtualHeight, SDL_LOGICAL_PRESENTATION_STRETCH);
            //SDL_GetWindowSize(m_context->window, &m_context->WindowWidth, &m_context->WindowHeight);      
            SDL_GetWindowSize(m_context->window, &m_context->WindowWidth, &m_context->WindowHeight);
            ImGuiIO& io = ImGui::GetIO();
            io.DisplaySize = ImVec2((float)m_context->WindowWidth, (float)m_context->WindowHeight);

            std::cout << m_context->WindowWidth << "," << m_context->WindowHeight << "\n";

            initScene();
            break;
        }
                
        if (event.type == SDL_EVENT_KEY_DOWN)
        {
            switch (event.key.key)
            {
            case SDLK_ESCAPE:
                m_context->f_running = false;
                break;

            case SDLK_P:
                f_paused = !f_paused;
                m_soundManager->play("pause.wav");
                break;

            case SDLK_E:
                m_stateID = StateID::Menu;
                m_soundManager->play("pause.wav");
                break;

            case SDLK_R:
                enter();
                m_soundManager->play("pause.wav");
                break;

            case SDLK_D:                
                f_debug = !f_debug;
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

                initScene();
                break;
            }
        }
    }

    const bool* keyboard = SDL_GetKeyboardState(nullptr);

    player1.diry = 0;
    player1.dirx = 0;

    player2.diry = 0;
    player2.dirx = 0;
    
    if (keyboard[SDL_SCANCODE_W])
    {
        player1.diry = -1;
    }
    if (keyboard[SDL_SCANCODE_S])
    {
        player1.diry = 1;
    }
    if (keyboard[SDL_SCANCODE_UP])
    {
        player2.diry = -1;
    }
    if (keyboard[SDL_SCANCODE_DOWN])
    {
        player2.diry = 1;
    }

    SDL_GetMouseState(&mouseX, &mouseY);

    return m_stateID;
}

void GameState::collisionPlayerBall(Player& player, Ball& ball)
{   
    //cs120PongPaddle
    //https://www.youtube.com/watch?v=32IOEHqjzsI        
    float maxDy = 1.5f;
   
    ball.dirx *= -1;

    float ballCenterY = ball.m_body.y + ball.m_body.h * 0.5f;
    float paddleCenterY = player.m_body.y + player.m_body.h * 0.5f;

    float diffy = ballCenterY - paddleCenterY;

    //std::cout << "diffy" << diffy << "\n";

    //normalized -0.5 to 0.5
    diffy /= player.m_body.h;
    //std::cout << "normalized diffy" << diffy << "\n";

    //-1 to 1
    diffy *= 2;
    //std::cout << "diffy*2" << diffy << "\n";

    diffy *= maxDy;

    ball.diry = diffy;

    m_soundManager->play("game_clik.wav");
}

void GameState::clampPlayer(Player& player)
{
    if (phy.collisionAABB(player.m_body, phy.m_walls[Wallid::Top]))
    {
        player.m_body.y = 0;
    }
    if (phy.collisionAABB(player.m_body, phy.m_walls[Wallid::Bottom]))
    {
        player.m_body.y = m_context->WindowHeight - player.m_body.h;
    }
}

void GameState::initScene()
{
    m_scaleX = (float)m_context->WindowWidth / m_context->virtualWidth;
    m_scaleY = (float)m_context->WindowHeight / m_context->virtualHeight;

    //=======================================//
    // Ball pos reset
    //=======================================//
    ball.reset((m_context->virtualWidth / 2) * m_scaleX, (m_context->virtualHeight / 2) * m_scaleY);

    //=======================================//
    // Player_1 pos reset
    //=======================================//
    player1.m_body.x = 50 * m_scaleX;
    player1.m_body.y = (m_context->virtualHeight / 2 - player1.m_body.h / 2) * m_scaleY;

    //=======================================//
    // Player_2 pos reset
    //=======================================//
    player2.m_body.x = (m_context->virtualWidth - 50) * m_scaleX;
    player2.m_body.y = (m_context->virtualHeight / 2 - player1.m_body.h / 2) * m_scaleY;
   
    //=======================================//
    // Player1 score text
    //=======================================//
    m_player1ScoreText.setText("0", m_context->renderer);
    m_player1ScoreText.setPosition(10 * m_scaleX, 10 * m_scaleY);
    m_player1ScoreText.setSize(50 * m_scaleX, 50 * m_scaleY);

    //=======================================//
    // Player2 score text
    //=======================================//
    m_player2ScoreText.setText("0", m_context->renderer);
    m_player2ScoreText.setPosition((m_context->virtualWidth - 70) * m_scaleX, 10 * m_scaleY);
    m_player2ScoreText.setSize(50 * m_scaleX, 50 * m_scaleY);
     
    //=======================================///-*/*-
    // Player_1 texture
    //=======================================//
    player1.m_body.w = player1.m_texture->w * m_scaleX;
    player1.m_body.h = player1.m_texture->h * m_scaleY;
    player1.speed = 400 * m_scaleY;

    //=======================================//
    // Player_2 texture
    //=======================================//
    player2.m_body.w = player2.m_texture->w * m_scaleX;
    player2.m_body.h = player2.m_texture->h * m_scaleY;
    player2.speed = 400 * m_scaleY;

    //=======================================//
    // ball texture 
    //=======================================//
    ball.m_body.w = ball.m_texture->w * m_scaleX;
    ball.m_body.h = ball.m_texture->h * m_scaleY;

    ball.speedX = 300 * m_scaleX;
    ball.speedY = 300 * m_scaleY;

    phy.m_walls[Wallid::Left] = 
    { 
        0, -1, 
        10, (float)m_context->virtualHeight* m_scaleY
    };

    phy.m_walls[Wallid::Top] = 
    { 
        0, -1, 
        (float)m_context->virtualWidth* m_scaleX, 1
    };

    phy.m_walls[Wallid::Right] = 
    { 
        ((float)m_context->virtualWidth - 10) * m_scaleX, 0,
        10, (float)m_context->virtualHeight* m_scaleY
    };

    phy.m_walls[Wallid::Bottom] = 
    { 
        0, (float)m_context->virtualHeight * m_scaleY, 
        (float)m_context->virtualWidth * m_scaleX, 0 
    };
}

//=======================================================//
// Lua
//=======================================================//

int GameState::lu_setPlayer1Pos(lua_State* ls)
{
    lua_getglobal(ls, "__game");

    GameState* game = static_cast<GameState*>(lua_touserdata(ls, -1));

    lua_pop(ls, 1);

    float x = (float)lua_tonumber(ls, -2);
    float y = (float)lua_tonumber(ls, -1);

    game->player1.m_body.x = x;
    game->player1.m_body.y = y;

    return 0;
}

int GameState::lu_setPlayer2Pos(lua_State* ls)
{
    lua_getglobal(ls, "__game");

    GameState* game = static_cast<GameState*>(lua_touserdata(ls, -1));

    lua_pop(ls, 1);

    float x = (float)lua_tonumber(ls, -2);
    float y = (float)lua_tonumber(ls, -1);

    game->player2.m_body.x = x;
    game->player2.m_body.y = y;

    return 0;
}

int GameState::lu_setBallPos(lua_State* ls)
{
    lua_getglobal(ls, "__game");

    GameState* game = static_cast<GameState*>(lua_touserdata(ls, -1));

    lua_pop(ls, 1);

    float x = (float)lua_tonumber(ls, -2);
    float y = (float)lua_tonumber(ls, -1);

    game->ball.m_body.x = x;
    game->ball.m_body.y = y;

    return 0;
}

