/// モーターの値を送信する例

#include <CanWriter.hpp>
#include <Message.hpp>  /// 標準的な構造体が定義されているヘッダ
 
CanWriter<sizeof(Message::Motor)> writer(0);

void setup() {
}

void loop() {
	
	/// 構造体をセットします
	/// 設定された送信量とセットされた型のサイズが異なる場合、static_assertによってコンパイルエラーになります
	writer.setMessage( Message::Motor { -125 } );

	/// CANバスにデータを配信
	writer.update();

	delay(1);
	
}
