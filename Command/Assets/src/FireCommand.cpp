#include "FireCommand.h"

namespace command_pattern
{
	void FireCommand::Execute(GameActor& actor)
	{
		actor.Fire();
	}
}

