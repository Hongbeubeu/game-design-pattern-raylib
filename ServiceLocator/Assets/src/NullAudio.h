#pragma once
#include "Audio.hpp"

class NullAudio : public Audio
{
public:
	virtual void PlaySound(int soundId)
	{
	}

	virtual void StopSound(int soundId)
	{
	}

	virtual void StopAllSound()
	{
	}
};

