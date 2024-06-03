#include "Observer.h"
#include "Subject.h"

int ConcreteObserver::staticId = 0;
int main()
{
	ConcreteSubject subject;

	ConcreteObserver observer1;
	ConcreteObserver observer2;
	ConcreteObserver observer3;

	subject.Attach(&observer1);
	subject.Attach(&observer2);
	subject.Attach(&observer3);


	std::cout << "Setting state to 1" << std::endl;
	subject.SetState(1);

	std::cout << "Detaching observer 2" << std::endl;
	subject.Detach(&observer2);

	std::cout << "Setting state to 2" << std::endl;
	subject.SetState(2);

	return 0;
}
