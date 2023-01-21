/// ボード : teensy4.0

#include <CanCommon.hpp>

CanBusTeensy<CAN1> bus;

// CanBusTeensy<CAN1>バスに、送信ID0として参加
CanWriter<Message::Motor> writer(bus, 0);

void setup() {
	
	bus.begin();
	
}

void loop() {

	// millisの戻り値をモーターの値として送信する
	writer.setMessage( { millis() } );

	if (writer)
	{
		// 送信値を表示
		writer.show('\n');
	}
	else
	{
		// 送信エラー
		Serial.println("error");
	}

	// バスを更新
	bus.update();

	delay(1);

}
