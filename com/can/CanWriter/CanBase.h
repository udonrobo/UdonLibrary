#pragma once

#define BOARD_TEENSY_4X

#if defined(BOARD_TEENSY_4X)
#	include <FlexCAN_T4.h>
#	include <intervalTimer.h>
#elif defined(HOGEHOGEHOGE)

#else
#	error "Not supported except teensy4.x"
#endif

#include "FunctionBinder.h"

struct Message_t {
	uint8_t id;
	uint8_t buf[8];
};

template<class T>  /// リンクエラー対策
class _CanBase {

	protected:

		using Can = FlexCAN_T4<CAN1, RX_SIZE_256, TX_SIZE_256>;
		static Can     can;
		static uint8_t id;

	public:

		static void begin() {
#if defined(BOARD_TEENSY_4X)
			can.begin();
			can.setClock(CLK_60MHz);
			can.setBaudRate(1000000);
			can.enableFIFO();
			can.enableFIFOInterrupt();
			can.onReceive(isr);

			static IntervalTimer timer;
			timer.begin([] {can.events();}, 200);
#else
			mcp2515.reset();
			mcp2515.setBitrate(CAN_125KBPS);
			mcp2515.setNormalMode();
			constexpr uint8_t interruptPin = 2;
			pinMode(interruptPin, INPUT_PULLUP);
			attachInterrupt(digitalPinToInterrupt(interruptPin), isr, CHANGE);
#endif
		}

	protected:
	
		/// @brief 受信処理
#if defined(BOARD_TEENSY_4X)
		static void isr(const CAN_message_t& input) {
			Message_t msg = {};
			for (uint8_t i = 0; i < 8; i++)
				msg.buf[i] = input.buf[i];
			FunctionBinder<const Message_t&>::bind(msg);
		}
#else
		static void isr() {
			Message_t msg = {};
			static can_frame input;
			if (mcp2515.readMessage(&input) == MCP2515::ERROR_OK) {
				msg.id = input.can_id;
				for (uint8_t i = 0; i < 8; i++)
					msg.buf[i] = input.data[i];
			}
			FunctionBinder<const Message_t&>::bind(msg);
		}
#endif

		/// @brief 送信処理
		/// @param msg 送信内容
		static void write(const Message_t& msg) {
#if defined(BOARD_TEENSY_4X)
			CAN_message_t output = {};
			output.id = msg.id;
			for (uint8_t i = 0; i < 8; i++)
				output.buf[i] = msg.buf[i];
			while (!can.write(output));  // 送信
#else
			static can_frame output;
			output.id = msg.id;
			for (uint8_t i = 0; i < 8; i++)
				output.data[i] = msg.buf[i];
			mcp2515.sendMessage(&output);
#endif
		}

};

using CanBase = _CanBase<void>;

template<class T> CanBase::Can _CanBase<T>::can;
template<class T> uint8_t      _CanBase<T>::id;
