//
//    非同期ストリーム読み取り
//
//    Copyright (c) 2022-2023 Okawa Yusuke
//    Copyright (c) 2022-2023 Udonrobo
//

#pragma once

#include <Udon/Types/StringView.hpp>

namespace Udon
{
    /// @brief 非同期ストリーム読み取り
    class AsyncStreamReader
    {

        Stream& istream;

        char  buffer[256];
        char* iterator;

    public:
        AsyncStreamReader(Stream& istream)
            : istream(istream)
            , buffer()
            , iterator(buffer)
        {
        }

        Udon::StringView readStringUntil(char terminate)
        {
            while (istream.available())
            {
                const auto read = istream.read();

                if (read == -1)
                {
                    continue;
                }

                if (iterator == std::end(buffer))
                {
                    break;
                }

                if (read == terminate)
                {
                    const auto end = iterator;
                    iterator       = buffer;
                    return { buffer, end };
                }

                *iterator = read;
                ++iterator;
            }

            return { buffer, (size_t)0 };
        }
    };

}    // namespace Udon
