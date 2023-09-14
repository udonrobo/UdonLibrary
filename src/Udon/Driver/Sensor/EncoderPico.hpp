#pragma once

#ifdef ARDUINO_ARCH_RP2040

#    include <Udon/Driver/Pio/StateMachineAllocator.hpp>
#    include <Udon/Driver/Pio/QuadratureEncoder.pio.hpp>

namespace Udon
{

    class EncoderPico
    {
        uint8_t pinA;
        uint8_t pinB;

        // 割り込み使用時とPIO使用時で使用する変数が異なる
        union
        {
            struct
            {
                int32_t count;
                uint8_t phase;
            } interrupt;    // 割り込み使用時

            Pio::StateMachine stateMachine;    // PIO使用時
        } u;

        enum class Mode : uint8_t
        {
            Unstarted,
            Interrupt,
            Pio,
        } mode;

    public:
        /// @brief コンストラクタ
        /// @param pinA エンコーダーのA相ピン
        /// @param pinB エンコーダーのB相ピン
        EncoderPico(uint8_t pinA, uint8_t pinB)
            : pinA(pinA)
            , pinB(pinB)
            , u()
            , mode()
        {
        }

        /// @brief 開始
        /// @param alwaysUseInterrupt 常に割り込みを使用する
        /// @remark alwaysUseInterrupt が false の場合、PIOの使用を試みる
        ///         A相とB相のピン番号が隣り合っている場合、PIOを使用可能
        ///         各PIOに別のプログラムをロードしている場合、PIOを使用できない
        ///         PIOを使用できない場合は割り込みを使用する
        void begin(bool alwaysUseInterrupt = false)
        {
            if (alwaysUseInterrupt)
            {
                beginInterrupt();
            }
            else
            {
                if (not beginStateMachine())
                {
                    beginInterrupt();
                }
            }
        }

        /// @brief エンコーダーの値を取得する
        /// @return エンコーダーの値
        int32_t read() const
        {
            switch (mode)
            {
            case Mode::Interrupt: return u.interrupt.count;
            case Mode::Pio: return Pio::Encoder::quadrature_encoder_get_count(u.stateMachine.pio, u.stateMachine.index);
            default: return 0;
            }
        }

        /// @brief エンコーダーの値を表示する
        void show() const
        {
            switch (mode)
            {
            case Mode::Interrupt: Serial.print("Int "); break;
            case Mode::Pio: Serial.print("PIO "); break;
            default: Serial.print("Unstarted!"); return;
            }
            Serial.print(read());
            Serial.print('\t');
        }

    private:
        /// @brief PIOを使用してエンコーダーを計測する
        /// @return 使用可能な場合 true
        bool beginStateMachine()
        {
            if (abs(pinA - pinB) != 1)
            {
                return false;    // A相とB相のピン番号が隣り合っている場合はPIOを使用できない
            }

            const auto pin = min(pinA, pinB);    // 小さいほうのピン番号を使用する

            // 使用可能なPIOにプログラムをロードする
            if (const auto sm = Pio::AllocateStateMachine(Pio::Encoder::quadrature_encoder_program))
            {
                u.stateMachine = *sm;
            }
            else
            {
                return false;    // 割り当て失敗
            }

            // 計測開始
            Pio::Encoder::quadrature_encoder_program_init(u.stateMachine.pio, u.stateMachine.index, u.stateMachine.offset, pin, 0);    // 最後の0は変えたほうが良いのかもしれない

            mode = Mode::Pio;
            return true;
        }

        /// @brief 割り込みを使用してエンコーダーを計測する
        void beginInterrupt()
        {
            pinMode(pinA, INPUT_PULLUP);
            pinMode(pinB, INPUT_PULLUP);
            attachInterruptParam(digitalPinToInterrupt(pinA), Interrupt, CHANGE, this);
            attachInterruptParam(digitalPinToInterrupt(pinB), Interrupt, CHANGE, this);
            mode = Mode::Interrupt;
        }

        /// @brief 割り込みハンドラ
        static inline void Interrupt(void* p)
        {
            auto self = static_cast<EncoderPico*>(p);

            static constexpr int8_t AddCountTable[16]{ 0, -1, 1, 2, 1, 0, -2, -1, -1, -2, 0, 1, 2, 1, -1, 0 };

            // 下位2ビットにA相とB相の状態を格納する
            self->u.interrupt.phase |= (gpio_get(self->pinA) << 1) | gpio_get(self->pinB);

            // カウントテーブルを参照してカウントを更新する
            self->u.interrupt.count += AddCountTable[self->u.interrupt.phase & 0b1111];

            // 2ビット上へシフトして過去のデータとする
            self->u.interrupt.phase <<= 2;
        }
    };
}    // namespace Udon

#endif    // defined(ARDUINO_ARCH_RP2040)