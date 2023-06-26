#pragma once

#include <udon/traits/MaybeInvoke.hpp>

namespace udon
{
    template <template <typename> typename Transmitter, typename Message, size_t N>
    class ArrayTransmitter
    {
        using transmitter_type = Transmitter<Message>;
        transmitter_type transmitter;

    public:
        ArrayTransmitter(const transmitter_type& transmitter)
            : transmitter(transmitter)
        {
        }

        void begin()
        {
            udon::MaybeInvoke_begin(transmitter);
        }

        void update()
        {
            udon::MaybeInvoke_update(transmitter);
        }

        void show()
        {
            udon::MaybeInvoke_show(transmitter);
        }

        Message& at(size_t index)
        {
            return transmitter[index];
        }

        const Message& at(size_t index) const
        {
            return transmitter[index];
        }        

        Message& operator[](size_t index)
        {
            return transmitter[index];
        }

        const Message& operator[](size_t index) const
        {
            return transmitter[index];
        }
        
    };

}    // namespace udon
