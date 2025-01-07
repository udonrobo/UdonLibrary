//
//    通信経由モーター制御クラス
//
//    Copyright (c) 2022 udonrobo
//

#pragma once

#include <Udon/Stl/EnableSTL.hpp>
#include <algorithm>

#include <Udon/Com/Message/Motor.hpp>
#include <Udon/Traits/HasMemberFunction.hpp>
#include <Udon/Utility/Show.hpp>
#include <Udon/Traits/ReaderWriterTraits.hpp>
#include <Udon/Types/Direction.hpp>

namespace Udon
{

    /// @brief 通信経由モーター制御クラス
    template <template <typename> class Writer>
    class MotorBy
    {
        static_assert(Traits::IsWriter<Writer>::value, "Writer is not writer");

        using WriterType = Writer<Message::Motor>;
        WriterType writer;

        int16_t power;    // 出力値 -255 ~ 255

        Udon::Direction direction;    // 回転方向

    public:

        /// @brief コンストラクタ
        /// @param writer 送信クラスオブジェクト
        /// @param direction 回転方向
        MotorBy(WriterType&& writer, Udon::Direction direction = Udon::Direction::Forward)
            : writer(std::move(writer))
            , power()
            , direction(direction)
        {
        }

        /// @brief 出力値を取得
        /// @param p 出力値
        void move(int16_t p)
        {
            power = Constrain(p, (int16_t)-255, (int16_t)255);
            writer.setMessage({ static_cast<int16_t>(power * Udon::DirectionToSign(direction)) });
            Udon::Traits::MaybeInvokeUpdate(writer);
        }

        /// @brief 停止
        void stop()
        {
            move(0);
        }

        /// @brief 出力値をシリアル出力
        void show() const
        {
            Udon::Show(power);
        }
    };

}    // namespace Udon