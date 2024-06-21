#pragma once
#include "Audio.hpp"

class ConsoleAudio final : public Audio
{
public:
	virtual void PlaySound(const int soundId)
	{
		std::cout << "[Console Audio]: " << "Play sound: " << soundId << std::endl;
	}

	virtual void StopSound(const int soundId)
	{
		std::cout << "[Console Audio]: " << "Stop sound: " << soundId << std::endl;
	}

	virtual void StopAllSound()
	{
		std::cout << "[Console Audio]: " << "Stop all sounds" << std::endl;
	}
};

