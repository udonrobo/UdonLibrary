//
//    通信経由モーター制御クラス
//
//    Copyright (c) 2022-2024 udonrobo
//

#pragma once

#include <Udon/Stl/EnableSTL.hpp>
#include <algorithm>

#include <Udon/Com/Message/Motor.hpp>
#include <Udon/Traits/HasMemberFunction.hpp>
#include <Udon/Common/Show.hpp>
#include <Udon/Traits/ReaderWriterTraits.hpp>

namespace Udon
{

    template <template <typename> typename Writer>
    class MotorBy
    {
        static_assert(Traits::IsWriter<Writer>::value, "Writer is not writer");

        using WriterType = Writer<Message::Motor>;
        WriterType writer;

        int16_t power;    // 出力値 -255 ~ 255

        bool direction;    // 回転方向 true: forward, false: backward

    public:
        MotorBy(WriterType&& writer, bool direction)
            : writer(std::move(writer))
            , power()
            , direction(direction)
        {
        }

        void move(int16_t p)
        {
            power = Constrain(p, -255, 255);
            writer.setMessage({ static_cast<int16_t>(power * (direction ? 1 : -1)) });
            Udon::Traits::MaybeInvokeUpdate(writer);
        }

        void stop()
        {
            move(0);
        }

        void show() const
        {
            Udon::Show(power);
        }
    };

}    // namespace Udon