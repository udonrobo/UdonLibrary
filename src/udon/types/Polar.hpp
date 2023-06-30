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
//    直交座標系
//
//-------------------------------------------------------------------


#pragma once

#include <Udon/Types/PolarFwd.hpp>
#include <Udon/Types/Vector2DFwd.hpp>

template <typename T>
inline Udon::Vector2D<T> Udon::Polar_impl<T>::toVector() const noexcept
{
    return { x(), y() };
}

namespace Udon
{
    using Polar = Udon::Polar_impl<double>;
}
