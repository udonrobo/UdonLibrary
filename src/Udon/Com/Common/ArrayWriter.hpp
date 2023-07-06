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
//    メッセージ配列送信クラス
//
//-------------------------------------------------------------------

#pragma once

#include <Udon/Traits/MaybeInvoke.hpp>
#include <Udon/Utility/Parsable.hpp>

namespace Udon
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
            Udon::MaybeInvoke_begin(writer);
        }

        void update()
        {
            writer.setMessage(messages);
            Udon::MaybeInvoke_update(writer);
        }

        void show() const
        {
            Udon::MaybeInvoke_show(writer);
        }

        void showRaw() const
        {
            Udon::MaybeInvoke_showRaw(writer);
        }

        writer_type& getWriter()
        {
            return writer;
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

}    // namespace Udon