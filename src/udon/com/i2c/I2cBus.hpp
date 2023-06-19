//-----------------------------------------------
//
//	UdonLibrary
//
//	Copyright (c) 2022-2023 Okawa Yusuke
//	Copyright (c) 2022-2023 udonrobo
//
//	Licensed under the MIT License.
//
//-----------------------------------------------
//
//  I2c バス
//
//-----------------------------------------------


#pragma once

#include <udon/algorithm/Delegate.hpp>

#ifdef UDON_TEENSY_I2C_SLAVE_MODE
#    include <i2c_driver.h>
#    include <i2c_driver_wire.h>
#else
#    include <Wire.h>
#endif

namespace udon
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
    class I2cBus_impl
        : public II2cBus
    {

        TwoWire& wire;

        const uint32_t timeoutMs;

        uint32_t transmitMs;

        uint32_t restartCount;

        /// @brief C言語スタイルのコールバック関数に メンバ関数を登録するためのデリゲート
        udon::Delegate<I2cBus_impl, void(int)>  onReceiveDelegate;
        udon::Delegate<I2cBus_impl, void(void)> onRequestDelegate;

        /// @brief C言語スタイルのコールバック関数
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
        // デリゲートによって I2cBus_impl::invokeOnReceive 呼び出し
        // ↓
        // userOnReceive(I2cBus_impl::onReceiveで登録したコールバック関数) 呼び出し

    public:
        I2cBus_impl(TwoWire& wire, uint32_t timeoutMs = 50)
            : wire(wire)
            , timeoutMs(timeoutMs)
            , transmitMs()
            , restartCount()
            , onReceiveDelegate(this, &I2cBus_impl::invokeOnReceive)
            , onRequestDelegate(this, &I2cBus_impl::invokeOnRequest)
            , userOnReceive()
            , userOnRequest()
        {
        }

        /// @brief I2cバスの有効性を取得
        explicit operator bool() const override
        {
            return millis() - transmitMs < timeoutMs;
        }

        /// @brief 更新
        bool update() override
        {
            if (not operator bool())
            {
                restart();
                return false;
            }
            return true;
        }

        void show() const override
        {
            Serial.print("restart: ");
            Serial.print(restartCount);
            Serial.print(" transmit[ms]: ");
            Serial.print(transmitMs);
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
            wire.end();
            wire.begin();
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
            wire.onReceive(onReceiveDelegate);
        }

        inline void onRequest(void (*function)()) override
        {
            userOnRequest = function;
            wire.onRequest(onRequestDelegate);
        }
    };

}    // namespace udon

/// @brief I2C バスのラッパークラス
#define I2cBus \
    I2cBus_impl<__COUNTER__>
