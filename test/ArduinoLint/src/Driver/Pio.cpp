//
//    構文テスト
//
//    Copyright (c) 2022-2024 udonrobo
//

#include <Arduino.h>
#include <Udon/Driver/Pio/PioClock.hpp>
#include <Udon/Driver/Pio/StateMachineAllocator.hpp>

#ifdef ARDUINO_ARCH_RP2040

__attribute__((unused)) static void test()
{
    (void)Udon::PioClockBegin(1, 16'000'000);

    (void)Udon::Pio::AllocateStateMachine(Udon::Pio::Sqwave::squarewave_program);
}

#endif