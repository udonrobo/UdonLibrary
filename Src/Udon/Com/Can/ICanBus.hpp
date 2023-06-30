#pragma once

#include <Udon/Com/Can/CanInfo.hpp>

namespace Udon
{

    class ICanBus
    {
    public:
        virtual ~ICanBus() {}

        virtual operator bool() const = 0;

        virtual Udon::CanNodeView createTxNode(uint32_t id, size_t size) = 0;

        virtual Udon::CanNodeView createRxNode(uint32_t id, size_t size) = 0;
    };
}    // namespace Udon