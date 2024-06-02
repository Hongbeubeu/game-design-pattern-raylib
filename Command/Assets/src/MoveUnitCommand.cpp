#include "MoveUnitCommand.h"

namespace CommandPattern
{
	void MoveUnitCommand::Execute()
	{
		prevX = unit->X();
		prevY = unit->Y();
		unit->MoveTo(x, y);
	}

	void MoveUnitCommand::Undo()
	{
		unit->MoveTo(prevX, prevY);
	}
}

