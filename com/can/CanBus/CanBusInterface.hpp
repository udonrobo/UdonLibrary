/// @brief バス管理クラスのインターフェースクラス
/// @remark バス管理クラスはCANBusInterfacクラスを適切にオーバーライドします。
///         CANBusInterfacクラスのメンバが全てのバス管理クラスに存在することが保証されるため、Writer, Readerクラスは全てのバス管理クラスを使用することができます。

#pragma once

#include "CANInfo.hpp"

class CANBusInterface {

	public:

		virtual void joinTX(CANNodeInfo&) = 0;
		virtual void joinRX(CANNodeInfo&) = 0;
		virtual void detachTX(CANNodeInfo&) = 0;
		virtual void detachRX(CANNodeInfo&) = 0;

};
