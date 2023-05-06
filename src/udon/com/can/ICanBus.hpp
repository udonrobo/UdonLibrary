#pragma once

#include <udon/com/can/CanInfo.hpp>

namespace udon
{

    class ICanBus
    {
	public:
        virtual ~ICanBus() {}

        virtual void begin(const uint32_t baudrate = 1000000) = 0;

        virtual void end() = 0;

        virtual void update(uint32_t transmissionIntervalMs) {}

        virtual udon::CanNodeView createTxNode(uint16_t id, size_t size) = 0;

        virtual udon::CanNodeView createRxNode(uint16_t id, size_t size) = 0;
    };
}    // namespace udon