#pragma once

#if defined(ARDUINO_ARCH_RP2040)

namespace Udon
{
    class EncoderPico
    {
        uint8_t pinA;
        uint8_t pinB;
        uint8_t phase;
        int32_t count;

    public:
        /// @brief コンストラクタ
        /// @param pinA エンコーダーのA相ピン
        /// @param pinB エンコーダーのB相ピン
        EncoderPico(uint8_t pinA, uint8_t pinB)
            : pinA(pinA)
            , pinB(pinB)
            , phase()
            , count()
        {
        }

        /// @brief 開始
        void begin()
        {
            pinMode(pinA, INPUT_PULLUP);
            pinMode(pinB, INPUT_PULLUP);
            attachInterruptParam(digitalPinToInterrupt(pinA), Interrupt, CHANGE, this);
            attachInterruptParam(digitalPinToInterrupt(pinB), Interrupt, CHANGE, this);
        }

        /// @brief エンコーダーの値を取得する
        /// @return エンコーダーの値
        int32_t read() const
        {
            return count;
        }

        /// @brief エンコーダーの値を表示する
        void show() const
        {
            Serial.print(read());
            Serial.print('\t');
        }

    private:
        /// @brief 割り込みハンドラ
        /// @param self
        static inline void Interrupt(void* p)
        {
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
        }
    };
}    // namespace Udon

#endif    // defined(ARDUINO_ARCH_RP2040)