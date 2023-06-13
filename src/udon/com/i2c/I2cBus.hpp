#pragma once

#ifdef ARDUINO

#include <udon/algorithm/Delegate.hpp>

#    ifdef UDON_TEENSY_I2C_SLAVE_MODE
#        include <i2c_driver.h>
#        include <i2c_driver_wire.h>
#    else
#        include <Wire.h>
#    endif

namespace udon
{

    /// @brief I2cBus クラスのインターフェース
    class II2cBus
    {
    public:
        virtual ~II2cBus() = default;

        virtual operator bool() const = 0;

        /// @brief 更新
        virtual bool update() = 0;

        /// @brief 以下メンバは Arduino TwoWire クラスと同等
        virtual void begin()                = 0;
        virtual void begin(uint8_t address) = 0;

        virtual void end() = 0;

        virtual void restart() = 0;

        virtual void setClock(uint32_t clock) = 0;

        virtual void beginTransmission(uint8_t address) = 0;

        virtual uint8_t endTransmission()                 = 0;
        virtual uint8_t endTransmission(uint8_t sendStop) = 0;

        virtual uint8_t requestFrom(uint8_t address, uint8_t quantity)                   = 0;
        virtual uint8_t requestFrom(uint8_t address, uint8_t quantity, uint8_t sendStop) = 0;

        virtual size_t write(uint8_t data)                         = 0;
        virtual size_t write(unsigned long data)                   = 0;
        virtual size_t write(long data)                            = 0;
        virtual size_t write(unsigned int data)                    = 0;
        virtual size_t write(int data)                             = 0;
        virtual size_t write(const uint8_t* data, size_t quantity) = 0;

        virtual int available() = 0;

        virtual int read() = 0;

        virtual int peek() = 0;

        virtual void flush() = 0;

        virtual void onReceive(void (*function)(int)) = 0;
        virtual void onRequest(void (*function)())    = 0;
    };

    /// @brief I2cBus クラス実装部
    /// @tparam Counter オブジェクト生成ごとにユニークな値を指定する(複数の I2C バスを使用する場合に必要)
    template <int Counter>
    class I2cBus_impl
        : public II2cBus
    {

        TwoWire& wire;

        const uint32_t timeoutMs;

        uint32_t lastTransmitMs;

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
                lastTransmitMs = millis();
                userOnReceive(n);
            }
        }
        void invokeOnRequest()
        {
            if (userOnRequest)
            {
                lastTransmitMs = millis();
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
            , lastTransmitMs()
            , onReceiveDelegate(this, &I2cBus_impl::invokeOnReceive)
            , onRequestDelegate(this, &I2cBus_impl::invokeOnRequest)
            , userOnReceive()
            , userOnRequest()
        {
        }

        /// @brief I2cバスの有効性を取得
        operator bool() const override
        {
            return millis() - lastTransmitMs < timeoutMs;
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

        /// @brief 以下メンバは TwoWire クラスと同等のメンバ
        inline void begin() override
        {
            wire.begin();
        }
        inline void begin(uint8_t address) override
        {
            wire.begin(address);
        }

        inline void end() override
        {
            wire.end();
        }

        inline void restart() override
        {
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
            return wire.endTransmission();
        }
        inline uint8_t endTransmission(uint8_t sendStop) override
        {
            return wire.endTransmission(sendStop);
        }

        inline uint8_t requestFrom(uint8_t address, uint8_t quantity) override
        {
            return wire.requestFrom(address, quantity);
        }
        inline uint8_t requestFrom(uint8_t address, uint8_t quantity, uint8_t sendStop) override
        {
            return wire.requestFrom(address, quantity, sendStop);
        }

        inline size_t write(uint8_t data) override
        {
            return wire.write(data);
        }
        inline size_t write(unsigned long data) override
        {
            return wire.write(data);
        }
        inline size_t write(long data) override
        {
            return wire.write(data);
        }
        inline size_t write(unsigned int data) override
        {
            return wire.write(data);
        }
        inline size_t write(int data) override
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
#    define I2cBus \
        I2cBus_impl<__COUNTER__>

#endif
