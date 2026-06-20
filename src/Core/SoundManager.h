#pragma once

#include <iostream>
#include <string>

#include <audio/miniaudio.h>

class SoundManager
{
public:
	SoundManager();
	~SoundManager();
	
	void play(std::string soundName);
	
private:
	ma_engine m_sfxPlayer;
};

