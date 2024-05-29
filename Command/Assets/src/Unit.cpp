#include "Unit.h"

#include "raylib.h"

void Unit::MoveTo(const int x, const int y) const
{
	TraceLog(LOG_INFO, "Unit moved to %d, %d", x, y);
}

int Unit::X() const
{
	return x;
}

int Unit::Y() const
{
	return y;
}

void Unit::SetY(const int destY)
{
	y = destY;
}


