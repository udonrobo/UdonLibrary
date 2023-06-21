//-----------------------------------------------
//
//	UdonLibrary
// 
//	Copyright (c) 2022-2023 Okawa Yusuke
//	Copyright (c) 2022-2023 udonrobo
//
//	Licensed under the MIT License.
//
//-----------------------------------------------
//
//  UART 送信クラス
//
//-----------------------------------------------


#pragma once

#include <udon/com/serialization/Serializer.hpp>
#include <udon/utility/Show.hpp>

namespace udon
{

    template <class Message>
    class UartWriter
    {

        Stream& uart;

        Message message;

    public:
        /// @brief コンストラクタ
        UartWriter(Stream& uart) noexcept
            : uart(uart)
            , message()
        {
        }

        /// @brief 更新
        void update()
        {
            for (auto&& it : udon::Pack(message))
            {
                uart.write(it);
            }
        }

        /// @brief 送信内容を更新
        /// @details 送信も行う
        /// @param rhs 送信内容
        void setMessage(const Message& rhs)
        {
            message = rhs;
        }

        /// @brief 送信内容を表示
        /// @param gap 区切り文字 (default: '\t')
        void show(char gap = '\t') const
        {
            udon::Show(message, gap);
        }

        /// @brief 生の送信内容を表示
        /// @param gap 区切り文字 (default: ' ')
        void showRaw(char gap = ' ') const
        {
            for (auto&& it : udon::Pack(message))
            {
                Serial.print(it);
                Serial.print(gap);
            }
        }
    };

}    // namespace udon
