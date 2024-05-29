#pragma once
#include "GameActor.h"

namespace CommandPattern
{
	class Command
	{
	public:
		virtual ~Command() = default;
		virtual void Execute(GameActor& actor) = 0;
	};
}

