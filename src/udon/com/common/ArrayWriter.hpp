#pragma once

#include <udon/traits/MaybeInvoke.hpp>
#include <udon/utility/Parsable.hpp>

namespace udon
{
    template <template <typename> typename Writer, typename Message, size_t N>
    class ArrayWriter
    {

        struct Data
        {
            Message datas[N];
            UDON_PARSABLE(datas);
        };

        Data messages;

        using writer_type = Writer<Data>;

        writer_type writer;

    public:
        ArrayWriter(writer_type&& writer)
            : writer(std::move(writer))
        {
        }

        void begin()
        {
            udon::MaybeInvoke_begin(writer);
        }

        void update()
        {
            writer.setMessage(messages);
            udon::MaybeInvoke_update(writer);
        }

        void show() const
        {
            udon::MaybeInvoke_show(writer);
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
