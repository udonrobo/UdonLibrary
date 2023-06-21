//-----------------------------------------------
//
//  UdonLibrary
//
//  Copyright (c) 2022-2023 Okawa Yusuke
//  Copyright (c) 2022-2023 udonrobo
//
//  Licensed under the MIT License.
//
//-----------------------------------------------
//
//  IM920 インターフェース
//
//-----------------------------------------------


#pragma once

#include <udon/stl/EnableSTL.hpp>

#include <vector>

#include <stdint.h>

namespace udon
{

    /// @brief IM920のインターフェース
    /// @details
    ///     IM920の通信を行うクラスはこのインターフェースを実装する
    ///     これによって、IM920の通信を行うクラスを差し替え(Im920 <-> Im920sl等)が簡単になる
    class IIm920
    {
    public:

        /// @brief デストラクタ
        virtual ~IIm920() = default;

        virtual operator bool() const = 0;

        /// @brief 送信バッファを登録する
        /// @param size 送信バッファのサイズ
        virtual std::vector<uint8_t>& registerSender(size_t size) = 0;

        /// @brief 受信バッファを登録する
        /// @param size 受信バッファのサイズ
        virtual const std::vector<uint8_t>& registerReceiver(size_t size) = 0;

    };
} // namespace udon
