//
//    I2c バス
//
//    Copyright (c) 2022-2023 udonrobo
//

#pragma once

#ifdef UDON_TEENSY_I2C_SLAVE_MODE
#    include <i2c_driver.h>
#    include <i2c_driver_wire.h>
#else
#    include <Wire.h>
#endif

namespace Udon
{

    /// @brief I2cBus クラスのインターフェース
    class II2cBus
    {
    public:
        virtual ~II2cBus() = default;

        /// @brief バスの有効性を取得
        virtual explicit operator bool() const = 0;

        /// @brief 更新
        virtual bool update() = 0;

        /// @brief バスの状態を表示
        virtual void show() const = 0;

        // 以下メンバは Arduino TwoWire クラスと同等

        virtual void begin() = 0;

        virtual void begin(uint8_t address) = 0;

        virtual void end() = 0;

        virtual void restart() = 0;

        virtual void setClock(uint32_t clock) = 0;

        virtual void beginTransmission(uint8_t address) = 0;

        virtual uint8_t endTransmission() = 0;

        virtual uint8_t endTransmission(uint8_t sendStop) = 0;

        virtual uint8_t requestFrom(uint8_t address, uint8_t quantity) = 0;

        virtual uint8_t requestFrom(uint8_t address, uint8_t quantity, uint8_t sendStop) = 0;

        virtual size_t write(uint8_t data) = 0;

        virtual size_t write(const uint8_t* data, size_t quantity) = 0;

        virtual int available() = 0;

        virtual int read() = 0;

        virtual int peek() = 0;

        virtual void flush() = 0;

        virtual void onReceive(void (*function)(int)) = 0;

        virtual void onRequest(void (*function)()) = 0;
    };

    /// @brief I2cBus クラス実装部
    /// @tparam Counter オブジェクト生成ごとにユニークな値を指定する(複数の I2C バスを使用する場合に必要)
    template <int Counter>
    class I2cBusImpl
        : public II2cBus
    {

        static I2cBusImpl* self;

        TwoWire& wire;

        const uint32_t timeoutMs;

        uint32_t transmitMs;

        uint32_t restartCount;

        /// @brief コールバック関数
        void (*userOnReceive)(int);
        void (*userOnRequest)();

        /// @brief 登録されたコールバック関数を呼び出す
        void invokeOnReceive(int n)
        {
            if (userOnReceive)
            {
                transmitMs = millis();
                userOnReceive(n);
            }
        }
        void invokeOnRequest()
        {
            if (userOnRequest)
            {
                transmitMs = millis();
                userOnRequest();
            }
        }

        // TwoWire::onReceive 割り込み発火
        // ↓
        // I2cBusImpl::invokeOnReceive 呼び出し
        // ↓
        // userOnReceive(I2cBusImpl::onReceiveで登録したコールバック関数) 呼び出し

    public:
        I2cBusImpl(TwoWire& wire, uint32_t timeoutMs = 100)
            : wire(wire)
            , timeoutMs(timeoutMs)
            , transmitMs()
            , restartCount()
            , userOnReceive()
            , userOnRequest()
        {
            self = this;
        }

        I2cBusImpl(const I2cBusImpl& rhs)
            : wire(rhs.wire)
            , timeoutMs(rhs.timeoutMs)
            , transmitMs(rhs.transmitMs)
            , restartCount(rhs.restartCount)
            , userOnReceive()
            , userOnRequest()
        {
            self = this;
        }

        /// @brief I2cバスの有効性を取得
        explicit operator bool() const override
        {
            return millis() - transmitMs < timeoutMs;
        }

#if defined(__MK64FX512__)    // Teensy 3.5 ではバスの再起動が適切に行えなかった。

        /// @brief 更新
        bool update() override
        {
            Serial.print(F("This board does not support I2C bus restart.\n"));
            return false;
        }

#else

        /// @brief 更新
        bool update() override
        {
            if (operator bool())
            {
                return true;
            }
            else
            {
                restart();
                return false;
            }
        }

#endif

        void show() const override
        {
            if (operator bool())
            {
                Serial.print(F("I2C: OK "));
            }
            else
            {
                Serial.print(F("I2C: NG "));
            }
            Serial.print(F("restart[n]: "));

#if defined(__MK64FX512__)
            Serial.print('-');
#else
            Serial.print(restartCount);
#endif

            Serial.print(F("\t transmit[ms]: "));
            Serial.print(transmitMs);
            Serial.print('\t');
        }

        /// @brief 以下メンバは TwoWire クラスと同等のメンバ
        inline void begin() override
        {
            wire.begin();
        }

        inline void begin(uint8_t address) override
        {
            wire.begin(address);
        }

#ifdef ARDUINO_ARCH_RP2040

        inline void setSDA(uint8_t pin)
        {
            wire.setSDA(pin);
        }

        inline void setSCL(uint8_t pin)
        {
            wire.setSCL(pin);
        }

#endif

        inline void end() override
        {
            wire.end();
        }

        inline void restart() override
        {
            ++restartCount;
            end();
            begin();
        }

        inline void setClock(uint32_t clock) override
        {
            wire.setClock(clock);
        }

        inline void beginTransmission(uint8_t address) override
        {
            wire.beginTransmission(address);
        }

        inline uint8_t endTransmission() override
        {
            return endTransmission(true);
        }

        inline uint8_t endTransmission(uint8_t sendStop) override
        {
            const auto ret = wire.endTransmission(sendStop);
            if (ret == 0)    // 送信成功
            {
                transmitMs = millis();
            }
            return ret;
        }

        inline uint8_t requestFrom(uint8_t address, uint8_t quantity) override
        {
            const auto ret = wire.requestFrom(address, quantity);
            if (ret == quantity)    // 送信成功
            {
                transmitMs = millis();
            }
            return ret;
        }

        inline uint8_t requestFrom(uint8_t address, uint8_t quantity, uint8_t sendStop) override
        {
            const auto ret = wire.requestFrom(address, quantity, sendStop);
            if (ret == quantity)    // 送信成功
            {
                transmitMs = millis();
            }
            return ret;
        }

        inline size_t write(uint8_t data) override
        {
            return wire.write(data);
        }

        inline size_t write(const uint8_t* data, size_t quantity) override
        {
            return wire.write(data, quantity);
        }

        inline int available() override
        {
            return wire.available();
        }

        inline int read() override
        {
            return wire.read();
        }

        inline int peek() override
        {
            return wire.peek();
        }

        inline void flush() override
        {
            wire.flush();
        }

        inline void onReceive(void (*function)(int)) override
        {
            userOnReceive = function;
            wire.onReceive([](int n)
                           { self->invokeOnReceive(n); });
        }

        inline void onRequest(void (*function)()) override
        {
            userOnRequest = function;
            wire.onRequest([]()
                           { self->invokeOnRequest(); });
        }
    };

    template <int Counter>
    I2cBusImpl<Counter>* I2cBusImpl<Counter>::self;

}    // namespace Udon

/// @brief I2C バスのラッパークラス
#define I2cBus \
    I2cBusImpl<__COUNTER__>
