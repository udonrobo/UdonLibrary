#pragma once

#include <udon/stl/EnableSTL.hpp>

#include <algorithm>

#include <udon/algorithm/DeltaTime.hpp>
#include <udon/com/message/Motor.hpp>
#include <udon/traits/MaybeInvoke.hpp>


namespace udon
{

    template <template <typename> typename Reader>
    class Encoder
    {
        using reader_type = Reader<message::Encoder>;

        reader_type reader;

        udon::DeltaTime deltaTime;

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
            udon::MaybeInvoke_update(reader);

            const auto prev = getCount();

            count = reader.getMessage() * (direction ? 1 : -1);

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
        void getSpeed() const
        {
            return speed;
        }

        /// @brief カウント値を取得
        /// @return カウント値
        void getCount() const
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

}    // namespace udon