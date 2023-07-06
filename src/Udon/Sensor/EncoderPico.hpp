#pragma once

#if defined(ARDUINO_ARCH_RP2040)

namespace Udon
{
    class EncoderPico
    {

        struct EncoderStatus
        {
            uint8_t pinA;
            uint8_t pinB;
            uint8_t phase;
            int32_t count;
        } status;

    public:

        /// @brief コンストラクタ
        /// @param pinA エンコーダーのA相ピン
        /// @param pinB エンコーダーのB相ピン
        EncoderPico(uint8_t pinA, uint8_t pinB)
            : status{ pinA, pinB, 0b0000, 0 }
        {
        }

        /// @brief 開始
        void begin()
        {
            AttachInterrupt(status.pinA, &status);
            AttachInterrupt(status.pinB, &status);
        }

        /// @brief エンコーダーの値を取得する
        /// @return エンコーダーの値
        int32_t read() const
        {
            return status.count;
        }

        /// @brief エンコーダーの値を表示する
        void show() const
        {
            Serial.print(read());
            Serial.print('\t');
        }

    private:

        /// @brief 割り込みハンドラ 
        /// @param status 
        static inline void Interrupt(EncoderStatus* status)
        {

            status->phase |= (gpio_get(status->pinA) << 1) | gpio_get(status->pinB);    // digitalRead は遅いので、レジスタを直接読み込む

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
            switch (status->phase & 0b1011)
            {
            case 0b0011: ++status->count; break;
            case 0b0010: --status->count; break;
            case 0b1001: --status->count; break;
            case 0b1000: ++status->count; break;
            }

            //  B-phase
            //  prevA  prevB  currA  currB  count
            //    -      0      1      1      -
            //    -      0      0      1      +
            //    -      1      1      0      +
            //    -      1      0      0      -
            switch (status->phase & 0b0111)
            {
            case 0b0011: --status->count; break;
            case 0b0001: ++status->count; break;
            case 0b0110: ++status->count; break;
            case 0b0100: --status->count; break;
            }

            status->phase <<= 2;    // current -> previous
        }

        // これ以下はattachInterruptにポインタ、関数オブジェクトなどを渡すことができないので生み出されたうんち
        // 各メンバ変数 `status` を指すポインタを 静的配列 `statuses` に格納することで、
        // 静的関数 Interrupt から `status` を参照できるようにしている

        static constexpr size_t PICO_GPOI_PIN_COUNT = 29;    // 0~28

        static EncoderStatus* statuses[PICO_GPOI_PIN_COUNT];

        static void AttachInterrupt(uint8_t pin, EncoderStatus* status)
        {

        // すでにきな臭いですね(?)
#    define RESISTOR_INTERRUPT(pin)   \
    case pin:                         \
        statuses[pin] = status;       \
        pinMode(pin, INPUT_PULLDOWN); \
        attachInterrupt(              \
            pin, [] { Interrupt(statuses[pin]); }, CHANGE)

            // お察しします
            switch (pin)
            {
                RESISTOR_INTERRUPT(0);
                RESISTOR_INTERRUPT(1);
                RESISTOR_INTERRUPT(2);
                RESISTOR_INTERRUPT(3);
                RESISTOR_INTERRUPT(4);
                RESISTOR_INTERRUPT(5);
                RESISTOR_INTERRUPT(6);
                RESISTOR_INTERRUPT(7);
                RESISTOR_INTERRUPT(8);
                RESISTOR_INTERRUPT(9);
                RESISTOR_INTERRUPT(10);
                RESISTOR_INTERRUPT(11);
                RESISTOR_INTERRUPT(12);
                RESISTOR_INTERRUPT(13);
                RESISTOR_INTERRUPT(14);
                RESISTOR_INTERRUPT(15);
                RESISTOR_INTERRUPT(16);
                RESISTOR_INTERRUPT(17);
                RESISTOR_INTERRUPT(18);
                RESISTOR_INTERRUPT(19);
                RESISTOR_INTERRUPT(20);
                RESISTOR_INTERRUPT(21);
                RESISTOR_INTERRUPT(22);
                RESISTOR_INTERRUPT(23);
                RESISTOR_INTERRUPT(24);
                RESISTOR_INTERRUPT(25);
                RESISTOR_INTERRUPT(26);
                RESISTOR_INTERRUPT(27);
                RESISTOR_INTERRUPT(28);
            }

#    undef RESISTOR_INTERRUPT
        }
    };
}    // namespace Udon

#endif    // defined(ARDUINO_ARCH_RP2040)