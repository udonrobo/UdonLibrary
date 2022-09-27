/// @file   FunctionBinder.h
/// @date   2022/09/26
/// @brief  クラスのメンバ関数を静的関数にバインドするクラス
/// @author 大河 祐介

#pragma once

template <class Ty>
struct remove_reference {
	using type = Ty;
};
template <class Ty>
struct remove_reference<Ty&> {
	using type = Ty;
};
template <class Ty>
struct remove_reference < Ty&& > {
	using type = Ty;
};

template< class Ty >
using remove_reference_t = typename remove_reference<Ty>::type;

template<class Ty>
inline decltype(auto) move(Ty&& value) {
	return static_cast < remove_reference_t<Ty> && > (value);
}
template<typename T>
inline decltype(auto) forward(remove_reference_t<T>& param)
{
	return static_cast < T && > (param);
}

template<class... PTy>
class FunctionBinder {

	public:

		/// @brief 単方向リスト
		template<class Ty>
		class List {
				struct Node {
					Ty value;
					Node* next;
				};
				Node* first = nullptr;  /// 最初の要素をさすポインタ
				Node* last = nullptr;   /// 最後の要素をさすポインタ
			public:

				~List() {
					Node* current = first;
					while (current) {
						const auto temp = current;
						current = current->next;
						delete temp;
					}
				}

				/// @brief 新しい要素を追加する
				inline List& operator<<(Ty&& r) {
					if (first)
						last = last->next = new Node{ r };
					else
						last = first = new Node{ r };
					return *this;
				}

				/// @brief すべての要素にアクセス
				/// @param CapTy 呼ばれる関数に渡す引数
				/// @param f(T&) それぞれの要素にアクセスする関数(引数に要素の参照が渡る)
				/// @param capture アクセス関数に渡す引数
				template<class... CapTy>
				inline void foreach (void (*f)(Ty&, CapTy&...), CapTy && ... capture) {
					Node* current = first;
					while (current) {
						f(current->value, forward<CapTy>(capture)...);
						current = current->next;
					}
				}

				List& operator=(List&& r) noexcept = delete;  /// 代入未実装
		};

		/// @brief 静的関数
		static void bind(PTy... param) {
			pList.foreach(accessor, move(param)...);
		}

	protected:

		/// @brief インスタンスを追加
		FunctionBinder() {
			pList << this;
		}

		/// @brief コールバックされる純粋仮想関数
		virtual void callback(PTy...) = 0;

	private :

		static List<FunctionBinder*> pList;

		static void accessor(FunctionBinder*& p, PTy... param) {  /// なぜかラムダ式にできない
			p->callback(param...);
		}
};

template<class... T>
FunctionBinder<T...>::List<FunctionBinder<T...>*> FunctionBinder<T...>::pList;
