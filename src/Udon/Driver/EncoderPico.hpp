//
//    Raspberry Pi Pico 用エンコーダークラス
//
//    Copyright (c) 2022-2024 udonrobo
//

#pragma once

#ifdef ARDUINO_ARCH_RP2040

#    include "Pio/StateMachineAllocator.hpp"
#    include "Pio/QuadratureEncoder.pio.hpp"

#    include <Udon/Algorithm/ScopedInterruptLocker.hpp>

namespace Udon
{

    /// @brief Raspberry Pi Pico 用エンコーダークラス
    class EncoderPico
    {
        uint8_t pinA;
        uint8_t pinB;

        Pio::StateMachine stateMachine;    // PIO使用時

    public:
        /// @brief コンストラクタ
        /// @param pinA エンコーダーのA相ピン
        /// @param pinB エンコーダーのB相ピン
        EncoderPico(uint8_t pinA, uint8_t pinB)
            : pinA(pinA)
            , pinB(pinB)
            , stateMachine()
        {
        }

        /// @brief 開始
        /// @note A相とB相のピン番号が隣り合っている場合、PIOを使用可能
        ///       各PIOに別のプログラムをロードしている場合、PIOを使用できない
        bool begin()
        {
            if (abs(pinA - pinB) != 1)
            {
                return false;    // A相とB相のピン番号が隣り合っていない場合はPIOを使用できない
            }

            // 使用可能なPIOにプログラムをロードする
            if (const auto sm = Pio::AllocateStateMachine(Pio::Encoder::quadrature_encoder_program))
            {
                stateMachine = *sm;
            }
            else
            {
                return false;    // 割り当て失敗
            }

            // 小さいほうのピン番号を使用する
            const auto pin = min(pinA, pinB);

            // 計測開始
            Pio::Encoder::quadrature_encoder_program_init(stateMachine.pio, stateMachine.index, stateMachine.offset, pin, 0);

            return true;
        }

        /// @brief エンコーダーの値を取得する
        /// @return エンコーダーの値
        int32_t read() const
        {
            return Pio::Encoder::quadrature_encoder_get_count(stateMachine.pio, stateMachine.index);
        }

        /// @brief エンコーダーの値を表示する
        void show() const
        {
            Serial.print(read());
        }
    };
}    // namespace Udon

#endif    // defined(ARDUINO_ARCH_RP2040)