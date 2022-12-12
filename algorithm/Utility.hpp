#pragma once

/// @brief  配列長を静的に取得
/// @param  配列
/// @return 配列長
template<class Ty, size_t Len>
inline constexpr size_t arrayLength(const Ty(&)[Len]) {
	return Len;
}