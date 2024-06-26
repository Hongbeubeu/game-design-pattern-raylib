#include "Unit.h"
#include "raylib.h"

void Unit::MoveTo(const int xValue, const int yValue)
{
	this->x = xValue;
	this->y = yValue;
	TraceLog(LOG_INFO, "Unit moved to %d, %d", xValue, yValue);
}

void Unit::Update() const
{
	DrawRectangle(x, y, 32, 32, RED);
}

int Unit::X() const
{
	return x;
}

int Unit::Y() const
{
	return y;
}

