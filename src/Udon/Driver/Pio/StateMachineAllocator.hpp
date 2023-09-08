#pragma once

#if defined(ARDUINO_ARCH_RP2040)

#    include <hardware/pio.h>
#    include <Udon/Stl/Optional.hpp>

namespace Udon
{
    namespace Pio
    {

        // PIOはpio0とpio1の2つがあり、それぞれ4つのステートマシンを持つ
        //
        // ------- pio0 ------  ------- pio1 ------
        // | state machine 0 |  | state machine 0 |
        // | state machine 1 |  | state machine 1 |
        // | state machine 2 |  | state machine 2 |
        // | state machine 3 |  | state machine 3 |
        // -------------------  -------------------

        struct StateMachine
        {
            PIO pio;      // pio0 or pio1
            int index;    // state machine index 0~3
            int offset;   // offset of the program
        };

        /// @brief 使用可能なステートマシンを検索し割り当てる
        /// @param program ステートマシンにロードするプログラム
        /// @return ステートマシンの割り当てに成功した場合はステートマシンの情報を返す それ以外はnulloptを返す
        Udon::Optional<StateMachine> AllocateStateMachine(const pio_program& program)
        {
            PIOProgram allocator(&program);

            StateMachine sm;

            if (not allocator.prepare(&sm.pio, &sm.index, &sm.offset))
            {
                return Udon::nullopt;
            }

            return sm;
        }

    }    // namespace Pio
}    // namespace Udon

#endif