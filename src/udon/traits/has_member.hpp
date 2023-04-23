#pragma once

#include <udon\stl\EnableSTL.hpp>

#include <type_traits>


/// @brief 特定のクラスに特定のメンバ関数が存在するかどうかをチェックする
#define UDON_MAKE_HAS_MEMBER(method)                                        \
	template<class T>													    \
	struct has_##method {												    \
	private:															    \
		template<class S, void(S::*)() = &S::method>					    \
		static std::true_type test(S*) { return {}; }					    \
		static std::false_type test(...) { return {}; }						\
	public:																    \
		static constexpr bool value = decltype(test((T*)nullptr))::value;   \
	};		                                                                \
	                                                                        \
	template<class T>                                                       \
	constexpr bool has_##method##_v = has_##method<T>::value;

namespace udon
{

	// udon::has_update<T>, udon::has_update_v<T>
	UDON_MAKE_HAS_MEMBER(update);

	// udon::has_show<T>, udon::has_show_v<T>
	UDON_MAKE_HAS_MEMBER(show);

}
