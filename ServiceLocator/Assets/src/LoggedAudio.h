#pragma once
#include <iostream>

#include "Audio.hpp"

class LoggedAudio : public Audio
{
public:
	LoggedAudio (Audio& wrapped) : wrapped(wrapped)
	{
	}
	virtual void PlaySound(const int soundId)
	{
		Log("PlaySound");
		wrapped.PlaySound(soundId);
	}
	virtual void StopSound(const int soundId)
	{
		Log("StopSound");
		wrapped.StopSound(soundId);
	}
	virtual void StopAllSound()
	{
		Log("stopAllSound");
		wrapped.StopAllSound();
	}

private:
	void Log(const char* message)
	{
		std::cout <<"[Logged Audio]: " << message << '\n';
	}
	Audio& wrapped;
};

