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
//    raspberry pi pico PIO clock
//
//-------------------------------------------------------------------


#pragma once

#if defined(ARDUINO_ARCH_RP2040)

#    include <Udon/Pio/SquareWave.pio.hpp>

namespace Udon
{

    inline void PicoPio0ClockBegin(uint8_t pin, double clockSpeed)
    {
        PIO pio = pio0;    // クロック生成用PIO

        // PIO設定(CANコントローラのクロック生成)
        for (unsigned int i = 0; i < count_of(Pio::Sqwave::squarewave_program_instructions); i++)
            pio->instr_mem[i] = Pio::Sqwave::squarewave_program_instructions[i];

        pio->sm[0].clkdiv  = (uint32_t)(F_CPU * 0.25f * (1 << 16) / clockSpeed);                              // 周波数設定(周波数低めのほうが誤差が少ない)
        pio->sm[0].pinctrl = (1 << PIO_SM0_PINCTRL_SET_COUNT_LSB) | (pin << PIO_SM0_PINCTRL_SET_BASE_LSB);    // PIOがIOピンにアクセスできるようにする

        gpio_set_function(pin, GPIO_FUNC_PIO0);
        hw_set_bits(&pio->ctrl, 1 << (PIO_CTRL_SM_ENABLE_LSB + 0));
    }

}    // namespace Udon

#endif