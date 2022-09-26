/// @file   Callbacker.h
/// @date   2022/09/26
/// @brief  クラスのメンバ関数を静的関数にバインドするクラス
/// @author 大河 祐介

/// 1つの関数のみ登録するモード(処理ちょっと軽い)
//#define SINGLE_CALLBACK

#pragma once

#ifdef SINGLE_CALLBACK

template<class T>
class _Callbacker {
	
		static _Callbacker* p;

	public:

		/// @brief 静的関数
		static inline void bind() {
			p->callback();
		}

	protected:
		/// インスタンスを追加
		_Callbacker() {
			p = this;
		}

		/// @brief コールバックされる仮想関数
		virtual void callback() = 0;
};
template<class T>
_Callbacker<T>* _Callbacker<T>::p;

#else

template<class T>
class _Callbacker {
		/// @brief 動的リストクラス
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
					while (current)
					{
						const auto temp = current;
						current = current->next;
						delete temp;
					}
				}

				inline List& operator<<(Ty&& r) {

					/// 新しいノードを作成
					if (first) {
						last->next = new Node{ r };
						last = last->next;
					}
					else {
						last = first = new Node{ r };
					}
					return *this;
				}

				/// @brief すべての要素にアクセス
				/// @param f(T&) それぞれの要素にアクセスするときに呼ばれる関数(引数に要素の参照が渡る)
				inline void foreach (void (*f)(Ty&)) {
					Node* current = first; 
					while (current) {
						f(current->value);
						current = current->next;
					}
				}

				List& operator=(List&& r) noexcept = delete;  /// 代入未実装
		};

		static List<_Callbacker*> pList;

	public:

		/// @brief 静的関数
		static inline void bind() {
			pList.foreach([](_Callbacker*& p) {
				p->callback();
			});
		}

	protected:
	
		/// インスタンスを追加
		_Callbacker() {
			pList << this;
		}

		/// @brief コールバックされる仮想関数
		virtual void callback() = 0;
		
};
template<class T>
_Callbacker<T>::List<_Callbacker<T>*> _Callbacker<T>::pList;

#endif

using Callbacker = _Callbacker<double/*ダミー*/>;
