#pragma once

#include <udon\stl\EnableSTL.hpp>

#include <algorithm>

#include <udon\com\can\CanReader.hpp>
#include <udon\com\message\PadPS5.hpp>

namespace udon
{

    template <class Reader>
    class PadPS5
    {

        Reader reader;

    public:
        PadPS5(Reader&& reader)
            : reader(std::move(reader))
        {
        }

        void update()
        {
        }
    };

    using CanPadPS5 = udon::PadPS5<udon::CanReader<udon::message::PadPS5>>;

}    // namespace udon
