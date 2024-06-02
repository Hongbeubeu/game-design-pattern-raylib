#pragma once
class Unit
{
public:
	int X() const;
	int Y() const;
	void MoveTo(int xValue, int yValue);
	void Update() const;
private:
	int x = 0, y = 0;
};

