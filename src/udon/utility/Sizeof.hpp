#pragma once

#include <stdint.h>
#include <udon\types\Float.hpp>

namespace udon
{

    template <class>
    inline constexpr size_t PackedSizeof() { return {}; }

    template<> inline constexpr size_t PackedSizeof<bool    >() { return sizeof(bool           ); }
    template<> inline constexpr size_t PackedSizeof<int8_t  >() { return sizeof(int8_t         ); }
    template<> inline constexpr size_t PackedSizeof<int16_t >() { return sizeof(int16_t        ); }
    template<> inline constexpr size_t PackedSizeof<int32_t >() { return sizeof(int32_t        ); }
    template<> inline constexpr size_t PackedSizeof<int64_t >() { return sizeof(int64_t        ); }
    template<> inline constexpr size_t PackedSizeof<uint8_t >() { return sizeof(uint8_t        ); }
    template<> inline constexpr size_t PackedSizeof<uint16_t>() { return sizeof(uint16_t       ); }
    template<> inline constexpr size_t PackedSizeof<uint32_t>() { return sizeof(uint32_t       ); }
    template<> inline constexpr size_t PackedSizeof<uint64_t>() { return sizeof(uint64_t       ); }
    template<> inline constexpr size_t PackedSizeof<float   >() { return sizeof(udon::float32_t); }
    template<> inline constexpr size_t PackedSizeof<double  >() { return sizeof(udon::float32_t); }

    template<class... Args>
    inline constexpr size_t PackedSizeof<Args&&...>()
    {

    }

}    // namespace udon
