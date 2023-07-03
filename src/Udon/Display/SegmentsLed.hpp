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
//    7セグメントLED
//
//-------------------------------------------------------------------

#pragma once

#include <Udon/Stl/EnableSTL.hpp>
#include <array>
#include <vector>
#include <stdint.h>

#if __has_include(<Arduino.h>)

#    include <Arduino.h>

namespace Udon
{

    /// @brief 7セグメントLEDを制御するクラス
    /// @details
    ///     ダイナミック制御により、複数の7セグメントLEDを制御します。
    ///     ループ毎に1桁ずつ表示します。
    class SegmentsLed
    {
        std::array<uint8_t, 7>  cathode;
        std::vector<uint8_t>    anode;
        std::array<uint8_t, 10> format;

        uint8_t value;    // 表示する値
        uint8_t digit;    // 表示中の桁

    public:
        /// @brief コンストラクタ
        /// @param cathode カソードのピン番号
        /// @param anode アノードのピン番号
        /// @param format 表示フォーマット(0~9)
        SegmentsLed(const std::array<uint8_t, 7>&  cathode,
                    const std::vector<uint8_t>&    anode,
                    const std::array<uint8_t, 10>& format)
            : cathode(cathode)
            , anode(anode)
            , format(format)
        {
            for (auto&& pin : cathode)
            {
                pinMode(pin, OUTPUT);
            }
            for (auto&& pin : anode)
            {
                pinMode(pin, OUTPUT);
            }
        }

        /// @brief 表示する値を設定
        /// @param num 表示する値
        void setValue(int num)
        {
            value = num;
        }

        /// @brief 表示
        void show()
        {
            // ループ毎に1桁ずつ表示
            const uint8_t single = static_cast<uint8_t>(value / pow(10, digit)) % 10;

            // 表示中の桁消灯
            digitalWrite(anode.at(digit), HIGH);

            digit++;
            if (digit >= anode.size())
            {
                digit = 0;
            }

            for (uint8_t i = 0; i < cathode.size(); i++)
            {
                digitalWrite(cathode.at(i), not bitRead(format.at(single), i));
            }

            // 桁点灯
            digitalWrite(anode.at(digit), LOW);
        }
    };

}    // namespace Udon

#endif