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
//    通信経由ロータリーエンコーダー
//
//-------------------------------------------------------------------

#pragma once

#include <Udon/Stl/EnableSTL.hpp>
#include <algorithm>

#include <Udon/Algorithm/DeltaTime.hpp>
#include <Udon/Com/Message/Motor.hpp>
#include <Udon/Traits/MaybeInvoke.hpp>

#include <Udon/Com/Traits.hpp>

namespace Udon
{

    template <template <typename> typename Reader>
    class Encoder
    {
        using reader_type = Reader<Message::Encoder>;

        reader_type reader;

        Udon::DeltaTime deltaTime;

        bool direction;

        int32_t count;

        int32_t offset;

        double speed;

    public:
        /// @brief コンストラクタ
        /// @param reader 受信クラスオブジェクト
        /// @param direction 回転方向
        Encoder(reader_type&& reader, bool direction)
            : reader(std::move(reader))
            , deltaTime()
            , direction(direction)
            , count()
            , offset()
            , speed()
        {
        }

        /// @brief 更新
        void update()
        {
            Udon::MaybeInvoke_update(reader);

            const auto prev = getCount();

            if (const auto countOpt = reader.getMessage())
            {
                count = countOpt->count * (direction ? 1 : -1);
            }

            const auto curr = getCount();

            speed = (curr - prev) / deltaTime.update().getDeltaTimeS();
        }

        /// @brief カウント値消去
        void reset()
        {
            offset = count;
        }

        /// @brief 速度を取得
        /// @return 速度[ppr/s]
        double getSpeed() const
        {
            return speed;
        }

        /// @brief カウント値を取得
        /// @return カウント値
        int32_t getCount() const
        {
            return count - offset;
        }

        /// @brief カウント値をシリアルポートに出力
        void show() const
        {
            Serial.print(getCount());
            Serial.print('\t');
        }
    };

}    // namespace Udon