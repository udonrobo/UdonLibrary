#include <iostream>


template<class MessageTy>
class CanReader
{

	uint8_t buffer[sizeof(MessageTy)];

public:

	template<class Bus>
	CanReader(Bus& bus)
		: buffer()
	{
		bus.joinReader(&CanReader::call, this);
	}

	void call() {

	}

	void show() {
		for (auto&& it : buffer) {
			std::cout << it << '\t';
		}
		std::cout << std::endl;
	}

};

#include <functional>
#include <vector>

template<int Bus>
class CanBusTeensy {

	std::vector<std::function<void()>> readers;

public:

	template<class MessageTy>
	void joinReader(void (CanReader<MessageTy>::*f)(), CanReader<MessageTy>* p)
	{
		readers.push_back(
			[&] {
				if (p)
				{
					p->f();
				}
			}
		);
	}


	void update() {
		for (auto&& it : readers)
		{
			it();
		}
	}

};


int main()
{
	CanBusTeensy<0> bus;
	CanReader<int> reader(bus);


	bus.update();

}
