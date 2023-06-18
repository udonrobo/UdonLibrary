#pragma once

#include <udon/com/can/CanInfo.hpp>

namespace udon
{

    class ICanBus
    {
    public:
        virtual ~ICanBus() {}

        virtual udon::CanNodeView createTxNode(uint16_t id, size_t size) = 0;

        virtual udon::CanNodeView createRxNode(uint16_t id, size_t size) = 0;
    };
}    // namespace udon