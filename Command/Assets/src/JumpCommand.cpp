#include "JumpCommand.h"

namespace command_pattern
{
	void JumpCommand::Execute(GameActor& actor)
	{
		actor.Jump();
	}
}

