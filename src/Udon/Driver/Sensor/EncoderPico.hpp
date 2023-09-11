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

        union
        {
            // 割り込み使用時
            struct
            {
                uint8_t phase;
                int32_t count;
            } interrupt;

            // PIO使用時
            Pio::StateMachine stateMachine;
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
            , mode(Mode::Unstarted)
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
        /// @return
        bool beginStateMachine()
        {
            if (abs(pinA - pinB) != 1)
            {
                return false;    // A相とB相のピン番号が隣り合っている場合はPIOを使用できない
            }

            const auto pin = pinA < pinB ? pinA : pinB;    // 小さいほうのピン番号を使用する

            // 使用可能なPIOにプログラムをロードする
            if (const auto smOpt = Pio::AllocateStateMachine(Pio::Encoder::quadrature_encoder_program))
            {
                u.stateMachine = *smOpt;
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
        /// @param self
        static inline void Interrupt(void* p)
        {
            noInterrupts();

            auto self = static_cast<EncoderPico*>(p);

            // static constexpr int8_t AddCountTable[16]{ 0, 1, -1, 2,
            //                                            -1, 0, -2, 1,
            //                                            1, -2, 0, -1,
            //                                            2, -1, 1, 0 };

            // static constexpr int8_t AddCountTable[16]{ 0, 1, -1, 2,
            //                                            -1, 0, -2, 1,
            //                                            1, -2, 0, -1,
            //                                            2, -1, 1, 0 };

            // state[3] = ((gpio_get(pin[3][0]) & 0x01) << 3) | ((gpio_get(pin[3][1]) & 0x01) << 2) | ((state[3] >> 2) & 0x03);
            // self->u.interrupt.phase |= (gpio_get(self->pinA) << 1) | gpio_get(self->pinB);

            // self->u.interrupt.count += AddCountTable[self->u.interrupt.phase];

            // self->u.interrupt.phase <<= 2;    // current -> previous

            //                _______         _______
            //       A ______|       |_______|       |______ A
            // - <--      _______         _______         __   --> +
            //       B __|       |_______|       |_______|   B

            //  A-phase
            //  prevA  prevB  currA  currB  count
            //    0      -      1      1      +
            //    0      -      1      0      -
            //    1      -      0      1      -
            //    1      -      0      0      +
            // switch (self->u.interrupt.phase & 0b1011)
            // {
            // case 0b0011: ++self->u.interrupt.count; break;
            // case 0b0010: --self->u.interrupt.count; break;
            // case 0b1001: --self->u.interrupt.count; break;
            // case 0b1000: ++self->u.interrupt.count; break;
            // }

            //  B-phase
            //  prevA  prevB  currA  currB  count
            //    -      0      1      1      -
            //    -      0      0      1      +
            //    -      1      1      0      +
            //    -      1      0      0      -
            // switch (self->u.interrupt.phase & 0b0111)
            // {
            // case 0b0011: --self->u.interrupt.count; break;
            // case 0b0001: ++self->u.interrupt.count; break;
            // case 0b0110: ++self->u.interrupt.count; break;
            // case 0b0100: --self->u.interrupt.count; break;
            // }

            interrupts();
        }
    };
}    // namespace Udon

#endif    // defined(ARDUINO_ARCH_RP2040)