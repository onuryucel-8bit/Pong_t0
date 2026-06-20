#include "SoundManager.h"

SoundManager::SoundManager()
{
	if (ma_engine_init(NULL, &m_sfxPlayer) != MA_SUCCESS)
	{
		std::cout << "ERROR:: sfxPlayer couldnt init\n";
	}
}

SoundManager::~SoundManager()
{
}

void SoundManager::play(std::string soundName)
{
	ma_engine_play_sound(&m_sfxPlayer, (std::string(cmake_PROJECT_RES) + "/audio/" + soundName).c_str(), NULL);
}
