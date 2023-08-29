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
#include <Udon/Traits/MaybeInvoke.hpp>
#include <Udon/Utility/Show.hpp>

namespace Udon
{

    template <template <typename> typename Writer>
    class Motor
    {
        using writer_type = Writer<Message::Motor>;

        writer_type writer;

        int16_t power;

        bool direction;

    public:
        Motor(writer_type&& writer, bool direction)
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

        void update()
        {
            const auto sendPower = power * (direction ? 1 : -1);
            writer.setMessage({ static_cast<int16_t>(sendPower) });
            Udon::MaybeInvoke_update(writer);
        }

        void show() const
        {
            Udon::Show(power);
        }
    };

}    // namespace Udon