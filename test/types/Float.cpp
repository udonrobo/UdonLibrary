#include <udon/types/Float.hpp>
#include <udon/com/serializer/Serializer.hpp>

inline void test()
{
    // シリアライズ
    {
        udon::float32_t a;
        static_assert(udon::CapacityWithChecksum(a) == 4 + 1, "");
        const auto b = udon::Pack(a);
        (void)udon::Unpack<udon::float32_t>(b);
    }
}