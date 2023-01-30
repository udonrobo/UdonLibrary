/// @brief バス管理クラスのインターフェースクラス
/// @remark バス管理クラスはCanBusInterfacクラスを適切にオーバーライドします。
///         CanBusInterfacクラスのメンバが全てのバス管理クラスに存在することが保証されるため、Writer, Readerクラスは全てのバス管理クラスを使用することができます。

#pragma once

#include "CanHandle.hpp"

class CanBusInterface {

	public:

		virtual void joinTX(CanNodeHandle&) = 0;
		virtual void joinRX(CanNodeHandle&) = 0;
		virtual void detachTX(CanNodeHandle&) = 0;
		virtual void detachRX(CanNodeHandle&) = 0;

};
