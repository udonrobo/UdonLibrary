/// ボード : teensy4.0

#include <CanCommon.hpp>

CanBusTeensy<CAN1> bus;

// CanBusTeensy<CAN1>バスに、監視ID0として参加
CanReader<Message::Motor> reader(bus, 0);

void setup() {
	
	bus.begin();
	
}

void loop() {

	// 受信内容表示
	reader.show('\n');

	// バスを更新
	bus.update();

	delay(1);

}
