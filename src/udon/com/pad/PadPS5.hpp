#pragma once

#include <udon\stl\EnableSTL.hpp>

#include <algorithm>
#include <type_traits>

#include <udon\algorithm\Input.hpp>

#include <udon\types\Position.hpp>
#include <udon\types\Vector2D.hpp>

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

        udon::Vector2D<int16_t> getLeftAxis() const { return {}; }
        udon::Vector2D<int16_t> getRightAxis() const { return {}; }

        udon::Position<int16_t> getStick() const
        {
            return {};
        }

        /// @brief reader クラスに update メンバがあるときの更新
        template<class Ty = Reader>
        typename std::enable_if<std::is_same<decltype(Ty::update), void(Ty::*)()>::value, void>::type
        update()
        {
            // reader.update();
            mainUpdate();
        }

        /// @brief reader クラスに update メンバがないときの更新
        template<class Ty = Reader>
        typename std::enable_if<!std::is_same<decltype(Ty::update), void(Ty::*)()>::value, void>::type
        update()
        {
            mainUpdate();
        }

        void mainUpdate()
        {

        }
    };

}    // namespace udon

#include <udon\com\can\CanReader.hpp>
#include <udon\com\message\PadPS5.hpp>

namespace udon
{

    using CanPadPS5 = udon::PadPS5<udon::CanReader<udon::message::PadPS5>>;

}    // namespace udon
