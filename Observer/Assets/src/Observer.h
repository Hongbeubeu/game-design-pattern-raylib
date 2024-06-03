#pragma once
#include <iostream>

// Observer interface
class Observer
{
public:
	virtual ~Observer() = default;
	virtual void Update(int state) = 0;
};

class ConcreteObserver : public Observer
{
private:
	int observerState;
	int id;
	static int staticId;
public:
	ConcreteObserver()
	{
		id = staticId++;
	}

	void Update(int state) override
	{
		observerState = state;
		std::cout << "Observer " << id << " updated with state " << observerState << std::endl;
	}
};
