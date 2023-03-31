#pragma once

#if defined(__MK20DX256__) || defined(__MK64FX512__) || defined(__MK66FX1M0__) || defined(__IMXRT1062__)
#	include <i2c_driver.h>
#	include <i2c_driver_wire.h>
#else
#	include <Wire.h>
#endif

namespace udon
{

	class I2cBus
	{

			TwoWire& wire;

			const uint32_t timeOutMs;

			uint32_t lastTransmissionMs;

		public:

			I2cBus(TwoWire& wire, uint32_t timeOutMs = 50)
				: wire(wire)
				, timeOutMs()
				, lastTransmissionMs()
			{}

			operator bool() const
			{
				return millis() - lastTransmissionMs < timeOutMs;
			}

			/// @brief 更新
			bool update(uint32_t limitMs = 50)
			{
				if(!operator bool())
				{
					end();
					begin();
					return false;
				}
				return true;
			}

			/// @brief 以下メンバは Arduino TwoWire クラスと同等
			inline void begin(               ) { wire.begin(       ); }
			inline void begin(uint8_t address) { wire.begin(address); }

			inline void end() { wire.end(); }

			inline void setClock(uint32_t clock) { wire.setClock(clock); }

			inline void beginTransmission(uint8_t address) { wire.beginTransmission(address); }

			inline uint8_t endTransmission(                ) { return wire.endTransmission(        ); }
			inline uint8_t endTransmission(uint8_t sendStop) { return wire.endTransmission(sendStop); }

			inline uint8_t requestFrom(uint8_t address, uint8_t quantity)                                                     { return wire.requestFrom(address, quantity);                            }
			inline uint8_t requestFrom(uint8_t address, uint8_t quantity, uint8_t sendStop)                                   { return wire.requestFrom(address, quantity, sendStop);                  }
			inline uint8_t requestFrom(uint8_t address, uint8_t quantity, uint32_t iaddress, uint8_t isize, uint8_t sendStop) { return wire.requestFrom(address, quantity, iaddress, isize, sendStop); }

			inline size_t write(uint8_t        data                 ) { return wire.write(data          ); }
			inline size_t write(unsigned long  data                 ) { return wire.write(data          ); }
			inline size_t write(long           data                 ) { return wire.write(data          ); }
			inline size_t write(unsigned int   data                 ) { return wire.write(data          ); }
			inline size_t write(int            data                 ) { return wire.write(data          ); }
			inline size_t write(const uint8_t *data, size_t quantity) { return wire.write(data, quantity); }

			inline int available() { return wire.available(); }

			inline int read() { return wire.read(); }

			inline int peek() { return wire.peek(); }

			inline void flush() { wire.flush(); }

			inline void onReceive (void (*function)(int )) { wire.onReceive(function); }
			inline void onRequest (void (*function)(void)) { wire.onRequest(function); }

	};

}