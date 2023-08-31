//-------------------------------------------------------------------
//
//    UdonLibrary
//
//    Copyright (c) 2022-2023 Okawa Yusuke
//    Copyright (c) 2022-2023 udonrobo
//
//    Licensed under the MIT License.
//
//-------------------------------------------------------------------
//
//    通信経由モーター制御クラス
//
//-------------------------------------------------------------------

#pragma once

#include <Udon/Stl/EnableSTL.hpp>
#include <algorithm>

#include <Udon/Com/Message/Motor.hpp>
#include <Udon/Traits/HasMemberFunction.hpp>
#include <Udon/Utility/Show.hpp>
#include <Udon/Traits/IsWriter.hpp>

namespace Udon
{

    template <template <typename> typename Writer>
    class MotorBy
    {
        static_assert(Traits::IsWriter<Writer>::value, "Writer is not writer");

        using writer_type = Writer<Message::Motor>;
        writer_type writer;

        int16_t power;    // 出力値 -255 ~ 255

        bool direction;    // 回転方向 true: forward, false: backward

    public:
        MotorBy(writer_type&& writer, bool direction)
            : writer(std::move(writer))
            , power()
            , direction(direction)
        {
        }

        void setPower(int16_t power)
        {
            this->power = power;
        }

        void setStop()
        {
            setPower(0);
        }

        int16_t getPower() const
        {
            return power;
        }

        void update()
        {
            const auto sendPower = power * (direction ? 1 : -1);
            writer.setMessage({ static_cast<int16_t>(sendPower) });
            Udon::Traits::MaybeInvokeUpdate(writer);
        }

        void stopUpdate()
        {
            writer.setMessage({ 0 });
            Udon::Traits::MaybeInvokeUpdate(writer);
        }

        void show() const
        {
            Udon::Show(power);
        }
    };

}    // namespace Udon