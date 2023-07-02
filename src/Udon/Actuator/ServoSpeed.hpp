//-------------------------------------------------------------------
//
//    UdonLibrary
//
//    Copyright (c) 2022-2023 fujimoto ryo
//    Copyright (c) 2022-2023 udonrobo
//
//    Licensed under the MIT License.
//
//-------------------------------------------------------------------
//
//    サーボの動く速さを調整するクラス
//    非常停止時に瞬時に止めることができる
//    稼働速度は選択できるが、速度をあげすぎるとサーボが追い付かなくなるので注意
//
//-------------------------------------------------------------------

#pragma once

#include <stdint.h>
#include <Udon/Math/Math.hpp>

namespace Udon
{
    enum class ServoChoose : uint8_t
    {
        _20kg,
        _30kg,
        _60kg,
    };

    class ServoSpeed
    {
    private:
        bool         status = true;
        unsigned int loopCycle, servo_preset = 0, target_angle = 0;
        float        move_angle = 0, next_angle = 0, value = 0, speed = 0, last_angle;

    public:
        /// @brief ServoSpeedのコンストラクタ
        /// @param LoopCycle 周期 us
        /// @param Choose servoの種類選択
        /// @param Last_Angle スタート時のサーボの角度
        ServoSpeed(unsigned int LoopCycle, ServoChoose Choose,
                   unsigned int Last_Angle = 0)
        {
            loopCycle  = LoopCycle / 100;
            last_angle = next_angle = static_cast<float>(Last_Angle);
            switch (Choose)
            {
            case ServoChoose::_20kg:
                // 20kgservo
                servo_preset = 180;
                break;
            case ServoChoose::_30kg:
                // 30kgservo
                servo_preset = 180;
                break;
            case ServoChoose::_60kg:
                // 60kgservo
                servo_preset = 270;
                break;
            }
        }
        /// @brief サーボの状態を更新する関数 毎周期実行する
        void update()
        {
            /*変化量の計算は少数で行い、計算後は整数型で返す*/
            last_angle = next_angle;    // 前周期のデータを保管しておく
            move_angle =
                static_cast<float>(target_angle) - static_cast<float>(last_angle);
            if (move_angle == 0)
            {
                status = true;
            }
            else
            {
                status = false;
                value  = Udon::Constrain(speed / static_cast<float>(loopCycle),
                                   Udon::Abs(move_angle) * -1, Udon::Abs(move_angle)) *
                        (move_angle < 0 ? -1 : 1);
                next_angle += value;
                next_angle = Udon::Constrain(next_angle, 0.0, static_cast<double>(servo_preset));
            }
        }

        /// @brief サーボの次の動きをセットする
        /// @param r 目標角度 度
        /// @param s 稼働速度 度/s
        void setAngle(unsigned int r, float s)
        {
            target_angle = r;
            speed        = s;
        }

        /// @brief 非常停止 今周期のデータを捨てる
        void stop() { next_angle = last_angle; }

        /// @brief サーボの状態を返す
        /// @return trueのときは停止状態
        bool getStatus() const { return status; }

        /// @brief 次の目標角度取得
        /// @return 目標角度 度
        unsigned int getAngle() const
        {
            return static_cast<unsigned int>(next_angle);
        }

        /// @brief 現在設定されているサーボの稼働速度を返す
        /// @return 現在速度 度/s
        float speedValue() const { return speed; }
    };
}    // namespace Udon
