#pragma once
class Unit
{
public:
	void MoveTo(int x, int y) const;
	int X() const;
	int Y() const;
	void SetY(int destY);

private:
	int x = 0, y = 0;
};

