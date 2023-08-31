#pragma once

#include <Udon/Stl/EnableSTL.hpp>
#include <type_traits>

#ifndef __cpp_lib_void_t
namespace std
{
    template <typename...>
    using void_t = void;
}
#endif
