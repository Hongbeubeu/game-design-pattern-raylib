#pragma once
#include <iostream>
class GameActor
{
public:
	explicit GameActor(const std::string& name);
	void Lurch() const;
	void Fire() const;
	void Jump() const;
	void Swap() const;
private:
	std::string name;
};

