/// @brief バス管理クラスのインターフェースクラス

#pragma once

#include <com\can\CanInfo.hpp>

class CanBusInterface
{

public:
    virtual void joinTX(CanNodeInfo&)   = 0;
    virtual void joinRX(CanNodeInfo&)   = 0;
    virtual void detachTX(CanNodeInfo&) = 0;
    virtual void detachRX(CanNodeInfo&) = 0;
};
