#include <gtest/gtest.h>
#include <Udon/Types/Vector2D.hpp>
#include <Udon/Serializer/Serializer.hpp>

TEST(Serialization, SerializedSize)
{
    EXPECT_EQ(Udon::SerializedSize<bool>(), sizeof(bool) + Udon::CRC8_SIZE);

    EXPECT_EQ(Udon::SerializedSize<uint8_t>(), sizeof(uint8_t) + Udon::CRC8_SIZE);

    EXPECT_EQ(Udon::SerializedSize<uint16_t>(), sizeof(uint16_t) + Udon::CRC8_SIZE);

    EXPECT_EQ(Udon::SerializedSize<uint32_t>(), sizeof(uint32_t) + Udon::CRC8_SIZE);

    EXPECT_EQ(Udon::SerializedSize<uint64_t>(), sizeof(uint64_t) + Udon::CRC8_SIZE);

    EXPECT_EQ(Udon::SerializedSize<float>(), sizeof(float) + Udon::CRC8_SIZE);

    EXPECT_EQ(Udon::SerializedSize<double>(), sizeof(Udon::float32_t) + Udon::CRC8_SIZE);

    EXPECT_EQ(Udon::SerializedSize<Udon::Vec2>(), sizeof(Udon::float32_t) * Udon::Vec2::Dimension + Udon::CRC8_SIZE);
}

TEST(Serialization, Serialize)
{
    Udon::Vec2 v{ 1.0, 2.0 };

    auto serialized = Udon::Serialize(v);

    EXPECT_EQ(Udon::SerializedSize<Udon::Vec2>(), serialized.size());                          // シリアライズ後のサイズが正しいか確認
    EXPECT_EQ(serialized.back(), Udon::CRC8({ serialized.data(), serialized.size() - 1 }));    // CRCが正しいか確認
}

TEST(Serialization, Deserialize)
{
    Udon::Vec2 v{ 1.0, 2.0 };

    auto deserialized = Udon::Deserialize<Udon::Vec2>(Udon::Serialize(v));

    EXPECT_TRUE(deserialized.has_value());    // デシリアライズに成功したか確認
    EXPECT_EQ(v, deserialized.value());       // デシリアライズ後の値が正しいか確認
}

TEST(Serialization, DeserializeError)
{
    Udon::Vec2 v{ 1.0, 2.0 };

    auto serialized = Udon::Serialize(v);

    serialized[1] = 0x00;    // CRCが不正になるように変更

    auto deserialized = Udon::Deserialize<Udon::Vec2>(serialized);

    EXPECT_FALSE(deserialized.has_value());
    EXPECT_NE(v, deserialized.value());
}
