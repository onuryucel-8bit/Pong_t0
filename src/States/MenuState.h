#pragma once

#include <iostream>

//-----libs-----//
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

#include <backends/imgui_impl_sdl3.h>
#include <backends/imgui_impl_sdlrenderer3.h>

#include <audio/miniaudio.h>
//--------------//

#include "Core/AppContext.h"
#include "Core/SoundManager.h"
#include "State.h"


class MenuState : public State
{
public:
	MenuState();
	~MenuState();

	void init(AppContext* appContext, SoundManager* soundManager);

	void enter() override;
	void exit() override;
	void update(float dt) override;
	void draw() override;
	void drawGui() override;
	StateID inputs() override;


private:
	AppContext* m_context = nullptr;
	SoundManager* m_soundManager = nullptr;

	StateID m_stateID = StateID::None;

	float scaleX;
	float scaleY;

};


