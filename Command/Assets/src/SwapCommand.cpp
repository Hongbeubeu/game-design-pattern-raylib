#include "SwapCommand.h"

namespace command_pattern
{
	void SwapCommand::Execute(GameActor& actor)
	{
		actor.Swap();
	}
}

