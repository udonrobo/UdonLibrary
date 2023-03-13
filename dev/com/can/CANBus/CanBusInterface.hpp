/// @brief バス管理クラスのインターフェースクラス
/// @remark バス管理クラスはCanBusInterfacクラスを適切にオーバーライドします。
///         CanBusInterfacクラスのメンバが全てのバス管理クラスに存在することが保証されるため、Writer, Readerクラスは全てのバス管理クラスを使用することができます。

#pragma once

#include "CanInfo.hpp"

class CanBusInterface {

	public:

		virtual void joinTX(CanNodeInfo&) = 0;
		virtual void joinRX(CanNodeInfo&) = 0;
		virtual void detachTX(CanNodeInfo&) = 0;
		virtual void detachRX(CanNodeInfo&) = 0;

};
