#pragma once

namespace udon
{

    template <template <typename> typename Reader>
    class Encoder
    {
        using reader_type = Reader<message::Encoder>;

        reader_type reader;

        int32_t count;

        bool direction;

    public:
        Encoder(reader_type&& reader, bool direction)
            : reader(std::move(reader))
            , direction(direction)
        {
        }

        void update()
        {
            reader.update();
            count = reader.getMessage() * (direction ? 1 : -1);
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