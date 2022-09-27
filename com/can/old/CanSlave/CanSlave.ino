/**
    CAN通信スレーブ(teensy用)
*/

#include "LoopCycleController.h"
#include "CanSlaveReader_t4.h"
#include "CanSlaveWriter_t4.h"

LoopCycleController loopCtrl(1000);

const uint8_t myID = 0;
const uint8_t dataSize = 30;
CanSlaveReader canr(myID, dataSize);
CanSlaveWriter canw(myID, dataSize);

void setup() {
	Serial.begin(115200);
	delay(1000);
}

void loop() {

	{	// 送信部
		for (uint8_t i = 0; i < canw.size(); i++)
			canw[i] = millis();
		canw.update();
	}

	{	// 受信部
		if (canr.isConnected())
			canr.show('\n');  // 受信値表示
		else
			Serial.println("unconnected");
	}


	loopCtrl.Update();
}


template<class T>
class List
{
		struct Node
		{
			T value;
			Node* next;
		};
		Node*  first;  /// 最初の要素をさすポインタ
		Node*  last ;  /// 最後の要素をさすポインタ
		size_t size ;

	public:

		List()
			: first(),
			  last (),
			  size ()
		{}

		~List()
		{
			Node* current = first;
			while (current)
			{
				const auto temp = current;
				current = current->next;
				delete temp;
			}
		}

		List& push_back(T&& value)
		{
			if (first)
			{
				last->next = new Node();
				last = last->next;
			}
			else /// 要素がない
			{
				last = first = new Node();
			}
			last->value = value;
			++size;
			return *this;
		}
		List& operator<<(T&& r) {
			return push_back(std::move(r));
		}

		void show()
		{
			Node* current = first;
			while (current)
			{
				current = current->next;
			}
		}

	private:
		List& operator=(List&& r) noexcept {}
};
