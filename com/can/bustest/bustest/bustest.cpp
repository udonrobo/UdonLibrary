#include <iostream>


template<class MessageTy>
class CanReader
{

	uint32_t id;
	uint8_t buffer[sizeof(MessageTy)];
	uint32_t lastTransmissionMs;

public:

	CanReader()
		: id()
		, buffer()
		, lastTransmissionMs()
	{}

	void show() {
		for (auto&& it : buffer) {
			std::cout << it << '\t';
		}
		std::cout << std::endl;
	}


	struct Handler {
		uint32_t id;
		uint8_t* buffer;
		size_t size;
		void* _this;
	};

	Handler getHandler() {
		return {
			id,
			buffer,
			sizeof buffer,
			this
		};
	}

};


#include <vector>

template<int Bus>
class CanBusTeensy {

	struct Node {
		uint16_t id;
		uint8_t* buffer;
		size_t size;
		void* _this;
	};
	std::vector<Node> readers;
	std::vector<Node> writers;

public:

	template<class MessageTy>
	void join(CanReader<MessageTy>& r) {
		for (auto&& it : readers) {
			if (&r == it._this) {
				return;
			}
		}
		auto&& hReader = r.getHandler();
		readers.push_back({
			static_cast<uint16_t>(hReader.id),
			hReader.buffer,
			hReader.size,
			hReader._this
			});
	}
	template<class MessageTy>
	void detach(const CanReader<MessageTy>& r)
	{
		for (auto&& it : readers)
		{
			if (it._this == &r)
			{
				readers.erase(it);
				break;
			}
		}
	}

	void update() {
		for (auto&& it : readers)
		{
			for (size_t i = 0; i < it.size; i++)
			{
				it.buffer[i] = 125;
			}
		}
	}

};


int main()
{
	CanBusTeensy<0> bus;
	CanReader<int> reader;
	CanReader<uint8_t> reader1;

	bus.join(reader);
	bus.join(reader1);

	bus.update();
}
