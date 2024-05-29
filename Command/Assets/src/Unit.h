#pragma once
class Unit
{
public:
	void MoveTo(int xValue, int yValue);
	int X() const;
	int Y() const;

private:
	int x = 0, y = 0;
};

