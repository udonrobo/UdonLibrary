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

        // 割り込み使用時
        uint8_t phase;
        int32_t count;

        // PIO使用時
        Pio::StateMachine stateMachine;

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
            , phase()
            , count()
            , stateMachine()
            , mode(Mode::Unstarted)
        {
        }

        /// @brief 開始
        /// @param alwaysUseInterrupt 常に割り込みを使用する
        /// @remark alwaysUseInterrupt が false の場合、PIOを使用できる場合はPIOを使用する
        ///         A相とB相のピン番号が隣り合っている場合、PIOを使用可能
        ///         PIOのステートマシン(8個)が全て使用中の場合はPIOを使用できない
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
            case Mode::Interrupt: return count;
            case Mode::Pio: return Pio::Encoder::quadrature_encoder_get_count(stateMachine.pio, stateMachine.index);
            default: return 0;
            }
        }

        /// @brief エンコーダーの値を表示する
        void show() const
        {
            switch (mode)
            {
            case Mode::Interrupt: Serial.print("Int \t"); break;
            case Mode::Pio: Serial.print("PIO \t"); break;
            default: Serial.print("Unstarted\t"); return;
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

            // 使用可能なステートマシンを割り当てる
            if (const auto smOpt = Pio::AllocateStateMachine(Pio::Encoder::quadrature_encoder_program))
            {
                stateMachine = *smOpt;
            }
            else
            {
                return false;    // 割り当て失敗
            }

            // ステートマシンへプログラムをロードする
            const uint offset = pio_add_program(stateMachine.pio, &Pio::Encoder::quadrature_encoder_program);

            // 計測開始
            Pio::Encoder::quadrature_encoder_program_init(stateMachine.pio, stateMachine.index, offset, pin, 0);    // 最後の0は変えたほうが良いのかもしれない

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

            self->phase |= (gpio_get(self->pinA) << 1) | gpio_get(self->pinB);    // digitalRead は遅いので、レジスタを直接読み込む

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
            switch (self->phase & 0b1011)
            {
            case 0b0011: ++self->count; break;
            case 0b0010: --self->count; break;
            case 0b1001: --self->count; break;
            case 0b1000: ++self->count; break;
            }

            //  B-phase
            //  prevA  prevB  currA  currB  count
            //    -      0      1      1      -
            //    -      0      0      1      +
            //    -      1      1      0      +
            //    -      1      0      0      -
            switch (self->phase & 0b0111)
            {
            case 0b0011: --self->count; break;
            case 0b0001: ++self->count; break;
            case 0b0110: ++self->count; break;
            case 0b0100: --self->count; break;
            }

            self->phase <<= 2;    // current -> previous

            interrupts();
        }
    };
}    // namespace Udon

#endif    // defined(ARDUINO_ARCH_RP2040)