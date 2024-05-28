#include "LurchCommand.h"

namespace command_pattern
{
	void LurchCommand::Execute(GameActor& actor)
	{
		actor.Lurch();
	}
}
