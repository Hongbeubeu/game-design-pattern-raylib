#include "MoveUnitCommand.h"

namespace CommandPattern
{
	void MoveUnitCommand::Execute()
	{
		unit->MoveTo(x, y);
	}
}

