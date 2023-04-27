#pragma once

#include <algorithm>    // std::move
#include <udon/algorithm/Input.hpp>
#include <udon/traits/HasMember.hpp>    // udon::has_update_v<T>
#include <udon/types/Position.hpp>
#include <udon/types/Vector2D.hpp>

namespace udon
{

    template <typename<typename> typename Reader>
    class PadPS5
    {

        Reader<udon::message::PadPS5> reader;

    public:

        PadPS5(Reader<udon::message::PadPS5>&& reader)
            : reader(std::move(reader))
        {
        }



        template<typename T = Reader>
        auto update() -> typename std::enable_if_t<udon::has_member_function_update_v<T>>::type
        {
            reader.update();
            mainUpdate();
        }
        template<typename T = Reader>
        auto update() -> typename std::enable_if_t<!udon::has_member_function_update_v<T>>::type
        {
            mainUpdate();
        }

        void mainUpdate()
        {
        }
    };

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
        template <class Ty = Reader>
        typename std::enable_if<udon::has_member_function_update_v<Ty>, void>::type
        update()
        {
            reader.update();
            mainUpdate();
        }

        /// @brief reader クラスに update メンバがないときの更新
        template <class Ty = Reader>
        typename std::enable_if<!udon::has_member_function_update_v<Ty>, void>::type
        update()
        {
            mainUpdate();
        }

        void mainUpdate()
        {
        }
    };

}    // namespace udon

#include <udon/com/can/CanReader.hpp>
#include <udon/com/uart/UartReader.hpp>
#include <udon/message/PadPS5.hpp>

namespace udon
{

    /// @brief CAN通信経由
    using CanPadPS5 = udon::PadPS5<udon::CanReader<udon::message::PadPS5>>;

    /// @brief UART通信経由
    using UartPadPS5 = udon::PadPS5<udon::UartReader<udon::message::PadPS5>>;

}    // namespace udon
