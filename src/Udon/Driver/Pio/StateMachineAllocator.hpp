#pragma once

#if defined(ARDUINO_ARCH_RP2040)

#    include <hardware/pio.h>
#    include <Udon/Types/Optional.hpp>
#    include <map>

namespace Udon
{
    namespace Pio
    {

        namespace Impl
        {
            /// @brief PIOの命令メモリを取得する
            /// @return PIOの命令メモリ
            inline std::map<const pio_program_t*, int>* GetPioInstructionMemory()
            {
                static std::map<const pio_program_t*, int> memory[2];
                return memory;
            }

        }    // namespace Impl

        /// @brief ステートマシンを表す構造体
        /// @details PIOはpio0とpio1の2つがあり、それぞれ4つのステートマシンを持つ
        ///         それぞれのステートマシンは各PIOの命令メモリ(Instruction Memory)に保存されている命令を実行する
        ///         ステートマシンとメインプログラムとは独立して動作し、32bit FIFOバッファを介してデータのやり取りを行う
        ///
        ///         --------- pio0 -------   -------- pio1 --------
        ///         |  state machine (0) |   |  state machine (0) |
        ///         |  state machine (1) |   |  state machine (1) |
        ///         |  state machine (2) |   |  state machine (2) |
        ///         |  state machine (3) |   |  state machine (3) |
        ///         | Instruction Memory |   | Instruction Memory |
        ///         ----------------------   ----------------------
        ///
        struct StateMachine
        {
            PIO pio;       // pio0 or pio1
            int index;     // state machine index 0~3
            int offset;    // offset of the program
        };

        /// @brief 使用可能なステートマシンを検索し割り当てる
        /// @param program ステートマシンにロードするプログラム
        /// @note プログラムのロード(pio_add_program)も行います
        /// @return ステートマシンの割り当てに成功した場合はステートマシンの情報を返す それ以外はnulloptを返す
        inline Udon::Optional<StateMachine> AllocateStateMachine(const pio_program& program)
        {
            PIO  pios[] = { pio0, pio1 };
            auto memory = Impl::GetPioInstructionMemory();

            // 既にプログラムメモリに同じプログラムがロードされている場合はそれを使用
            for (int i = 0; i < 2; i++)
            {
                auto p = memory[i].find(&program);
                if (p == memory[i].end())
                {
                    continue;
                }

                const int index = pio_claim_unused_sm(pios[i], false);
                if (index == -1)
                {
                    continue;
                }

                return StateMachine{ pios[i], index, p->second };
            }

            // 新しいプログラムをプログラムメモリにロード
            for (int i = 0; i < 2; i++)
            {
                if (pio_can_add_program(pios[i], &program))
                {
                    const int index = pio_claim_unused_sm(pios[i], false);
                    if (index == -1)
                    {
                        continue;
                    }

                    const int offset = pio_add_program(pios[i], &program);
                    // 失敗するとパニック

                    memory[i].insert({ &program, offset });

                    return StateMachine{ pios[i], index, offset };
                }
            }

            return Udon::nullopt;
        }

        /// @brief 確保したステートマシンを解放する
        /// @param sm ステートマシン
        inline void FreeStateMachine(const StateMachine& sm)
        {
            pio_sm_unclaim(sm.pio, sm.index);
        }

        /// @brief ステートマシンの情報を表示する
        /// @param sm ステートマシン
        inline void ShowStateMachineInfo(const StateMachine& sm)
        {
            if (sm.pio == pio0)
            {
                Serial.print("pio0");
            }
            else if (sm.pio == pio1)
            {
                Serial.print("pio1");
            }
            else
            {
                Serial.print("pio?");
                return;
            }

            Serial.print(" sm = ");
            Serial.print(sm.index);

            Serial.print(" offset = ");
            Serial.print(sm.offset);
        }

    }    // namespace Pio
}    // namespace Udon

#endif