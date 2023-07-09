#pragma once

#include <Udon/Com/Can/CanNode.hpp>

namespace Udon
{

    class ICanBus
    {
    public:
        virtual ~ICanBus() {}

        virtual operator bool() const = 0;

        virtual void joinTx(CanNode& node) = 0;

        virtual void joinRx(CanNode& node, void (*onReceive)(void*), void* p) = 0;

        virtual void leaveTx(CanNode& node) = 0;

        virtual void leaveRx(CanNode& node) = 0;

    };
}    // namespace Udon