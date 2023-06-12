#pragma once

#include <udon/stl/EnableSTL.hpp>

#include <algorithm>

#include <udon/message/Motor.hpp>
#include <udon/traits/MaybeInvoke.hpp>

namespace udon
{

    template <template <typename> typename Reader>
    class Encoder
    {
        using reader_type = Reader<message::Encoder>;

        reader_type reader;

        int32_t count;

		double speed;

        bool direction;

    public:
        Encoder(reader_type&& reader, bool direction)
            : reader(std::move(reader))
			, count()
			, speed()
            , direction(direction)
        {
        }

        void update()
        {
			udon::MaybeInvoke_update(reader);
            count = reader.getMessage() * (direction ? 1 : -1);
        }

		void getSpeed() const
		{

		}

        void getCount() const
        {
            return count;
        }

        void show() const
        {
            Serial.print(getCount());
            Serial.print('\t');
        }
    };

}    // namespace udon