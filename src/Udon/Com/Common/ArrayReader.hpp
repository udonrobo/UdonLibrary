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
//    メッセージ配列受信クラス
//
//-------------------------------------------------------------------

#pragma once

#include <Udon/Traits/MaybeInvoke.hpp>
#include <Udon/Utility/Parsable.hpp>

namespace Udon
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
            Udon::MaybeInvoke_begin(reader);
        }

        void update()
        {
            Udon::MaybeInvoke_update(reader);
            if (const auto message = reader.getMessage())
            {
                this->messages = *message;
            }
        }

        void show() const
        {
            Udon::MaybeInvoke_show(reader);
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
