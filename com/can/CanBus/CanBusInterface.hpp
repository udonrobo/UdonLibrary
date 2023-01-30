/// @brief バス管理クラスのインターフェースクラス
/// @remark バス管理クラスはCanBusInterfacクラスを適切にオーバーライドします。
///         CanBusInterfacクラスのメンバが全てのバス管理クラスに存在することが保証されるため、Writer, Readerクラスは全てのバス管理クラスを使用することができます。

#pragma once

class CanBusInterface {
	
	public:
	
		virtual void joinWriter() = 0;
		virtual void detachWriter() = 0;

		virtual void joinReader() = 0;
		virtual void detachReader() = 0;

};
