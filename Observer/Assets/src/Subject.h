#pragma once

#include <vector>
// Subject interface
class Subject
{
public:
	virtual ~Subject() = default;
	virtual void Attach(Observer* observer) = 0;
	virtual void Detach(Observer* observer) = 0;
	virtual void Notify() = 0;
};

class ConcreteSubject : public Subject
{
private:
	std::vector<Observer*> observers;
	int state;
public:
	int GetState() const
	{
		return state;
	}

	void SetState(int state)
	{
		this->state = state;
		Notify();
	}

	void Attach(Observer* observer) override
	{
		observers.push_back(observer);
	}

	void Detach(Observer* observer) override
	{
		observers.erase(std::remove(observers.begin(), observers.end(), observer), observers.end());
	}

	void Notify() override
	{
		for (auto observer : observers)
		{
			observer->Update(state);
		}
	}
};

