#pragma once
#include "Audio.hpp"
#include "NullAudio.h"

class Locator
{
public:
	static void Initialize() { service = &nullService; }
	static Audio& GetAudio() { return *service; }
	static void Provide(Audio* serviceProvider)
	{
		if (serviceProvider == nullptr)
		{
			service = &nullService;
		}
		else
		{
			service = serviceProvider;
		}
	}

private:
	static Audio* service;
	static NullAudio nullService;
};

