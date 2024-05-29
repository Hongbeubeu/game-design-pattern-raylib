#include "MoveUnitCommand.h"

namespace CommandPattern
{
	void MoveUnitCommand::Execute()
	{
		xBefore = unit->X();
		yBefore = unit->Y();
		unit->MoveTo(x, y);
	}

	void MoveUnitCommand::Undo()
	{
		unit->MoveTo(xBefore, yBefore);
	}

	void MoveUnitCommand::SetPosition(const int xValue, const int yValue)
	{
		this->x = xValue;
		this->y = yValue;
	}
}

