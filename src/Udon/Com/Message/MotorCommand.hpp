//
//    通信用メッセージ
//

#pragma once

#include <stdint.h>
#include <Udon/Traits/EnumerableMacro.hpp>

namespace Udon
{
    namespace Message
    {

        /// @brief モータの動作モード
        enum class ControlMode : uint8_t
        {
            EmergencyStop = 0,    // 非常停止
            HoldPosition  = 1,    // 現在位置保持
            Position      = 2,    // 位置制御モード
            Velocity      = 3,    // 速度制御モード
            PWM           = 4,    // PWM駆動モード
        };

        /// @brief モータに送信するコマンド
        struct MotorCommand
        {
            /// @brief モータの動作モード
            ControlMode mode;

            /// @brief モータの目標値
            float targetValue;

#ifdef ARDUINO
            /// @brief デバッグ出力
            void show() const
            {
                Serial.print(F("mode: ")), Serial.print(static_cast<int>(mode)), Serial.print('\t');
                Serial.print(F("targetValue: ")), Serial.print(targetValue), Serial.print('\t');
            }
#endif

            UDON_ENUMERABLE(mode, targetValue);
        };


        /// @brief モータからのフィードバック情報
        struct MotorFeedback
        {
            /// @brief 現在の位置
            float currentPosition;

            /// @brief 現在の速度
            float currentVelocity;

#ifdef ARDUINO
            /// @brief デバッグ出力
            void show() const
            {
                Serial.print(F("currentPosition: ")), Serial.print(currentPosition), Serial.print('\t');
                Serial.print(F("currentVelocity: ")), Serial.print(currentVelocity), Serial.print('\t');
            }
#endif

            UDON_ENUMERABLE(currentPosition, currentVelocity);
        };


        /// @brief モータの制限値やパラメータ
        struct MotorSetting
        {
            /// @brief 最大速度制限
            float maxVelocity;

            /// @brief 最大加速度制限
            float maxAcceleration;

            /// @brief モータのリセットフラグ
            bool reset;

#ifdef ARDUINO
            /// @brief デバッグ出力
            void show() const
            {
                Serial.print(F("maxVelocity: ")), Serial.print(maxVelocity), Serial.print('\t');
                Serial.print(F("maxAcceleration: ")), Serial.print(maxAcceleration), Serial.print('\t');
                Serial.print(F("reset: ")), Serial.print(reset), Serial.print('\t');
            }
#endif

            UDON_ENUMERABLE(maxVelocity, maxAcceleration, reset);
        };

    }    // namespace Message

}    // namespace Udon
