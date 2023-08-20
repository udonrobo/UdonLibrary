//-------------------------------------------------------------------
//
//    UdonLibrary
//
//    Copyright (c) 2022-2023 Okawa Yusuke
//    Copyright (c) 2022-2023 udonrobo
//
//    Licensed under the MIT License.
//
//-------------------------------------------------------------------
//
//    CAN 通信
//
//-------------------------------------------------------------------

#pragma once

/// @brief バスクラス
// #include "CanBusPico.hpp"
#include <Udon/Com/Can/CanBusPico.hpp>
#include <Udon/Com/Can/CanBusSpi.hpp>
#include <Udon/Com/Can/CanBusTeensy.hpp>

/// @brief 仮想ノードクラス
#include <Udon/Com/Can/CanReader.hpp>
#include <Udon/Com/Can/CanWriter.hpp>
