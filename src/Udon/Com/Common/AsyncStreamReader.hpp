#pragma once

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

        Udon::ArrayView<const char> readStringUntil(char tarminate)
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

                if (read == tarminate)
                {
                    const auto end = iterator;
                    iterator       = buffer;
                    return { buffer, end };
                }

                *iterator = read;
                ++iterator;
            }

            return { buffer, 0 };
        }
    };

}    // namespace Udon
