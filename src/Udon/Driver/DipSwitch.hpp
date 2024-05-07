//
//    DIPスイッチ
//
//    Copyright (c) 2022-2023 udonrobo
//

#pragma once

#include <Udon/Stl/EnableSTL.hpp>
#include <vector>
#include <stdint.h>

#ifdef ARDUINO

namespace Udon
{

    /// @brief DIPスイッチから得られる二進数値を十進数値に変換します
    /// @param pins ピン番号のリスト(下位ビットから順に)
    /// @note DecodeDipSwitch({ 2, 3, 4, 5 }) のように使用します
    /// @return 十進数値
    inline int DecodeDipSwitch(std::initializer_list<uint8_t> pins)
    {
        for (auto&& pin : pins)
        {
            pinMode(pin, INPUT_PULLUP);
        }

        int dec = 0;
        int i   = 0;

        for (auto&& pin : pins)
        {
            bitWrite(dec, i++, not digitalRead(pin));
            delayMicroseconds(5);  // チャタリング対策
        }

        return dec;
    }

}    // namespace Udon

#endif