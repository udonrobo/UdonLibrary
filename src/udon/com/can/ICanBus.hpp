#pragma once

#include <udon/com/can/CanInfo.hpp>

namespace udon
{

    class ICanBus
    {
    public:
        virtual ~ICanBus() {}

        virtual operator bool() const = 0;

        virtual udon::CanNodeView createTxNode(uint32_t id, size_t size) = 0;

        virtual udon::CanNodeView createRxNode(uint32_t id, size_t size) = 0;
    };
}    // namespace udon