#pragma once

#include <iostream>
#include <vector>

//-----libs-----//
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>

#include <backends/imgui_impl_sdl3.h>
#include <backends/imgui_impl_sdlrenderer3.h>

#include <audio/miniaudio.h>
//--------------//

#include <lua.hpp>

#include "State.h"
#include "MenuState.h"

#include "Core/AppContext.h"
#include "Core/SoundManager.h"
#include "Core/Physics.h"
#include "Core/AssetManager.h"

#include "Entity/Player.h"
#include "Entity/Ball.h"

#include "utils/Text.h"

class GameState : public State
{

public:
	GameState();

	void init(AppContext* appContext, SoundManager* soundManager);

	void enter() override;
	void exit() override;
	void update(float dt) override;
	void draw() override;
	void drawGui() override;
	StateID inputs() override;


private:
	
	static int lu_setPlayer1Pos(lua_State* ls);
	static int lu_setPlayer2Pos(lua_State* ls);
	static int lu_setBallPos(lua_State* ls);

	
	void collisionPlayerBall(Player& player, Ball& ball);
	void clampPlayer(Player& player);

	

	void initScene();

	AppContext* m_context = nullptr;
	SoundManager* m_soundManager = nullptr;
	lua_State* ls = nullptr;
	Physics phy;
	AssetManager m_assetManager;

	Player player1;
	Player player2;
	Ball ball;
	std::vector<Entity*> m_entityList;

	Text m_player1ScoreText;
	Text m_player2ScoreText;
	Text m_pausedText;

	StateID m_stateID = StateID::None;

	bool f_paused = false;
	bool f_debug = false;
	
	float m_scaleX = 1;
	float m_scaleY = 1;

	float mouseX = 0;
	float mouseY = 0;

	int player1Score = 0;
	int player2Score = 0;

};