//
//    Raspberry Pi Pico用PWMサーボドライバ
//
//    Copyright (c) 2022-2024 udonrobo
//

#pragma once

#if defined(ARDUINO_ARCH_RP2040)

#    include "pico/stdlib.h"
#    include "hardware/pwm.h"

namespace Udon
{
    class Servo
    {
    private:
        uint8_t  pin;
        uint     slice_num;
        uint     pwmChannel;
        uint32_t writeTime;

    public:
        void attach(uint8_t servoPin)
        {
            pin        = servoPin;
            pwmChannel = pin % 2 == 0 ? PWM_CHAN_A : PWM_CHAN_B;
            gpio_set_function(pin, GPIO_FUNC_PWM);
            slice_num = pwm_gpio_to_slice_num(pin);
            pwm_set_clkdiv(slice_num, F_CPU / 1000000);    // 分周比の設定
            pwm_set_wrap(slice_num, 19999);                // PWMの最大値
            pwm_set_enabled(slice_num, true);              // PWM有効
        }
        void write(uint8_t angle)
        {
            writeMicroseconds(map(angle, 0, 180, 500, 2500));
        }
        void writeMicroseconds(unsigned long time)
        {
            writeTime = time;
            pwm_set_chan_level(slice_num, pwmChannel, writeTime + 1);
        }
        // debug
        unsigned long readMicroseconds() const
        {
            return writeTime;
        }
        uint8_t read() const
        {
            return map(readMicroseconds(), 500, 2500, 0, 180);
        }
    };
}    // namespace Udon

#else
#    include <Servo.h>
#endif
