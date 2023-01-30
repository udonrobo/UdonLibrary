#pragma once

struct CanNodeHandle {
	uint32_t id         ;   // node id
	uint8_t* buffer     ;   // point buffer array
	size_t   length     ;   // buffer length
	uint32_t timestampUs;   // last buffer interrupt access time [μs]
};

struct CanBusErrorHandle {
	uint8_t  TXErrorCount;
	uint8_t  RXErrorCount;
	uint32_t timestampUs ;
	operator bool() const {
		return TXErrorCount < 100 &&
		       RXErrorCount < 100 &&
		       micros() - timestampUs < 5000;
	}
};
