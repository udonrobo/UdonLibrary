#pragma once

#include <udon/traits/MaybeInvoke.hpp>
#include <udon/utility/Parsable.hpp>

namespace udon
{
    template <template <typename> typename Reader, typename Message, size_t N>
    class ArrayReader
    {

        struct Data
        {
            Message datas[N];
            UDON_PARSABLE(datas);
        };

        Data messages;

        using reader_type = Reader<Data>;

        reader_type reader;

    public:
        ArrayReader(reader_type&& reader)
            : reader(std::move(reader))
        {
        }

        void begin()
        {
            udon::MaybeInvoke_begin(reader);
        }

        void update()
        {
            udon::MaybeInvoke_update(reader);
            if (const auto message = reader.getMessage())
            {
                this->messages = *message;
            }
        }

        void show() const
        {
            udon::MaybeInvoke_show(reader);
        }

        Message& at(size_t index)
        {
            return messages.datas[index];
        }

        const Message& at(size_t index) const
        {
            return messages.datas[index];
        }

        Message& operator[](size_t index)
        {
            return at(index);
        }

        const Message& operator[](size_t index) const
        {
            return at(index);
        }
    };

}    // namespace udon
