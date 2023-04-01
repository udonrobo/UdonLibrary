#pragma once

#include <udon\stl\EnableSTL.hpp>
#include <type_traits>

#include <udon\metapro\has_member.hpp>

namespace udon
{

	template<class T>
	auto Show(const T& rhs) -> typename std::enable_if<std::is_scalar<T>::value, void>::type
	{
        Serial.print(rhs);
    }

	template<class T>
	auto Show(const T& rhs) -> typename std::enable_if<udon::has_show<T>::value, void>::type
	{
        rhs.show();
	}

} // namespace udon
