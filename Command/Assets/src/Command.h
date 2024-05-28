#pragma once
#include "GameActor.h"

namespace command_pattern
{
	class Command
	{
	public:
		virtual ~Command() = default;
		virtual void Execute(GameActor& actor) = 0;
	};
}

