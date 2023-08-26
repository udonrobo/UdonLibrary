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

#ifdef ARDUINO

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
        /// ピン配置 cathode { 0, 1, 2, 3, 4, 5, 6 }
        ///     0
        ///  5     1
        ///     6
        ///  4     2
        ///     3
        SegmentsLed(std::array<uint8_t, 7>&&  cathode,
                    std::vector<uint8_t>&&    anode,
                    std::array<uint8_t, 10>&& format = {
                        0b0111111,    // 0
                        0b0000110,    // 1
                        0b1011011,    // 2
                        0b1001111,    // 3
                        0b1100110,    // 4
                        0b1101101,    // 5
                        0b1111101,    // 6
                        0b0100111,    // 7
                        0b1111111,    // 8
                        0b1101111     // 9
                    })
            : cathode(cathode)
            , anode(std::move(anode))
            , format(format)
        {
        }

        /// @brief 開始
        void begin()
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