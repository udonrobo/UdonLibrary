#pragma once

#include <Udon/Types/ArrayView.hpp>

namespace Udon
{
    /// @brief  16進数文字をバイト値に変換する
    /// @param  hex 変換元の16進数文字
    /// @return 変換後のバイト値 (変換に失敗した場合は0xff)
    uint8_t HexCharToByteValue(char hex) noexcept
    {
        // clang-format off
        switch (hex)
        {
        case '0':           return 0x0;
        case '1':           return 0x1;
        case '2':           return 0x2;
        case '3':           return 0x3;
        case '4':           return 0x4;
        case '5':           return 0x5;
        case '6':           return 0x6;
        case '7':           return 0x7;
        case '8':           return 0x8;
        case '9':           return 0x9;
        case 'A': case 'a': return 0xa;
        case 'B': case 'b': return 0xb;
        case 'C': case 'c': return 0xc;
        case 'D': case 'd': return 0xd;
        case 'E': case 'e': return 0xe;
        case 'F': case 'f': return 0xf;
        default: return 0xff;
        }
        // clang-format on
    }

    /// @brief  バイト値を16進数文字に変換する
    /// @param  byte 変換元のバイト値
    /// @return 変換後の16進数文字 (変換に失敗した場合は' ')
    char ByteValueToHexChar(uint8_t byte) noexcept
    {
        switch (byte)
        {
        case 0x0: return '0';
        case 0x1: return '1';
        case 0x2: return '2';
        case 0x3: return '3';
        case 0x4: return '4';
        case 0x5: return '5';
        case 0x6: return '6';
        case 0x7: return '7';
        case 0x8: return '8';
        case 0x9: return '9';
        case 0xa: return 'a';
        case 0xb: return 'b';
        case 0xc: return 'c';
        case 0xd: return 'd';
        case 0xe: return 'e';
        case 0xf: return 'f';
        default: return ' ';
        }
    }

    /// @brief  バイト列を16進数文字列に変換する
    /// @param  byteString 変換元のバイト列
    /// @param  distHexString 変換した16進数文字列の格納先
    /// @note   byteStringのサイズを基に16進数文字列を構築する (distHexStringのサイズはbyteStringのサイズの2倍以上である必要がある)
    /// @note   変換に失敗した場合はdistHexStringには何も書き込まれない
    /// @return 変換に成功した場合はtrue, 失敗した場合はfalse
    bool ByteStringToHexString(
        Udon::ArrayView<const uint8_t> byteString,
        Udon::ArrayView<uint8_t>       distHexString) noexcept
    {

        if (distHexString.size() < byteString.size() * 2)
        {
            return false;
        }

        auto it = distHexString.begin();
        for (const auto& byte : byteString)
        {
            *it++ = ByteValueToHexChar(byte >> 4 & 0xf);
            *it++ = ByteValueToHexChar(byte >> 0 & 0xf);
        }

        return true;
    }

    /// @brief  16進数文字列をバイト列に変換する
    /// @param  hexString 変換元の16進数文字列
    /// @param  distByteString 変換したバイト列の格納先
    /// @note   hexStringのサイズを基にバイト列を構築する (distByteStringのサイズはhexStringのサイズの1/2以上である必要がある)
    /// @note   変換に失敗した場合はdistByteStringには何も書き込まれない
    /// @return 変換に成功した場合はtrue, 失敗した場合はfalse
    bool HexStringToByteString(
        Udon::ArrayView<const uint8_t> hexString,
        Udon::ArrayView<uint8_t>       distByteString) noexcept
    {
        if (hexString.size() % 2)
        {
            return false;
        }

        if (distByteString.size() < hexString.size() / 2)
        {
            return false;
        }

        for (const auto& hex : hexString)
        {
            if (isxdigit(hex) == 0)
            {
                return false;
            }
        }

        for (size_t i = 0; i < hexString.size(); i += 2)
        {
            const uint8_t high    = HexCharToByteValue(hexString[i + 0]);
            const uint8_t low     = HexCharToByteValue(hexString[i + 1]);
            distByteString[i / 2] = high << 4 | low;
        }

        return true;
    }

}    // namespace Udon