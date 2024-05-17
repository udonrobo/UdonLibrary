//
//    IM920 インターフェース
//
//    Copyright (c) 2022-2024 udonrobo
//

#pragma once

#include <Udon/Stl/EnableSTL.hpp>
#include <vector>
#include <stdint.h>

#include <Udon/Types/ArrayView.hpp>

namespace Udon
{

    struct Im920Node
    {
        uint8_t* data;          // バッファを指すポインタ
        uint8_t  size;          // バッファの長さ
        uint32_t transmitMs;    // 最終通信時刻
    };

    /// @brief IM920のインターフェース
    /// @details
    ///     IM920の通信を行うクラスはこのインターフェースを実装する
    ///     これによって、IM920クラス(Im920, Im920s, ...)を一様に扱うことができる
    class IIm920
    {
    public:
        virtual ~IIm920() {}

        /// @brief バスの有効性を取得
        virtual operator bool() const = 0;

        /// @brief 送信ノードを登録
        /// @param node
        virtual void joinTx(Im920Node& node) = 0;

        /// @brief 受信ノードを登録
        virtual void joinRx(Im920Node& node) = 0;
    };
}    // namespace Udon
