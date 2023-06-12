#pragma once

#include <udon/stl/EnableSTL.hpp>

#include <algorithm>

#include <udon/message/Motor.hpp>
#include <udon/traits/MaybeInvoke.hpp>

namespace udon
{

    template <template <typename> typename Writer>
    class Motor
    {
        using writer_type = Writer<message::Motor>;

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

        void update()
        {
            const auto sendPower = power * (direction ? 1 : -1);
            writer.setMessage({ static_cast<int16_t>(sendPower) });
            udon::MaybeInvoke_update(writer);
        }

        void show() const
        {
            Serial.print(power);
            Serial.print('\t');
        }
    };

}    // namespace udon