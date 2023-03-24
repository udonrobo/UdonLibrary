/// @brief バス管理クラスのインターフェースクラス

#pragma once

#include <udon\com\can\CanInfo.hpp>

namespace udon
{

    class CanBusInterface
    {

    public:
        virtual void joinTX(CanNodeInfo&)   = 0;
        virtual void joinRX(CanNodeInfo&)   = 0;
        virtual void detachTX(CanNodeInfo&) = 0;
        virtual void detachRX(CanNodeInfo&) = 0;
    };

}    // namespace udon