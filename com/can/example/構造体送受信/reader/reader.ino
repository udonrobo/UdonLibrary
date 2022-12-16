/// モーターの値を受信する例

#include <CanReader.hpp>
#include <Message.hpp>  /// 標準的な構造体が定義されているヘッダ

CanReader<sizeof(Message::Motor)> reader(0);

void setup() {
	Serial.begin(115200);
}

void loop() {

	/// 受信は割り込みによって行われます

	/// 構造体を取得
	/// バイト列から構造体に変換するため、明示的に型を指定する必要があります
	/// 設定された送信量とセットされた型のサイズが異なる場合、static_assertによってコンパイルエラーになります
	const auto data = reader.getMessage<Message::Motor>();

	/// 表示
	Serial.println(data.power);

	delay(1);

}
