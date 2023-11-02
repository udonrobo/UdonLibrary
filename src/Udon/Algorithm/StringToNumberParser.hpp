#pragma once

#include <string>
#include <Udon/Traits/AlwaysFalse.hpp>

namespace Udon
{
    template <typename T>
    struct StringToNumberParser
    {
        static_assert(Traits::AlwaysFalse<T>::value, "T is not parsable.");
    };

    template <>
    struct StringToNumberParser<int>
    {
        static int Parse(const std::string& string) { return std::stoi(string); }
    };

    template <>
    struct StringToNumberParser<long>
    {
        static long Parse(const std::string& string) { return std::stol(string); }
    };

    template <>
    struct StringToNumberParser<long long>
    {
        static long long Parse(const std::string& string) { return std::stoll(string); }
    };

    template <>
    struct StringToNumberParser<unsigned int>
    {
        static unsigned int Parse(const std::string& string) { return std::stoul(string); }
    };

    template <>
    struct StringToNumberParser<unsigned long>
    {
        static unsigned long Parse(const std::string& string) { return std::stoul(string); }
    };

    template <>
    struct StringToNumberParser<unsigned long long>
    {
        static unsigned long long Parse(const std::string& string) { return std::stoull(string); }
    };

    template <>
    struct StringToNumberParser<float>
    {
        static float Parse(const std::string& string) { return std::stof(string); }
    };

    template <>
    struct StringToNumberParser<double>
    {
        static double Parse(const std::string& string) { return std::stod(string); }
    };

    template <>
    struct StringToNumberParser<long double>
    {
        static long double Parse(const std::string& string) { return std::stold(string); }
    };

}    // namespace Udon