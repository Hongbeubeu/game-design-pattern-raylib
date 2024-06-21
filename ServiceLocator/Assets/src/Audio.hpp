#pragma once
class Audio
{
public:
	virtual ~Audio()
	{
	}

	virtual void PlaySound(int soundId) = 0;
	virtual void StopSound(int soundId) = 0;
	virtual void StopAllSound() = 0;
};

