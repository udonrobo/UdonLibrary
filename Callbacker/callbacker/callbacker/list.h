#pragma once

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
	/// @param capture アクセッサ関数に渡す引数
	template<class... CapTy>
	inline void foreach(void (*f)(Ty&, CapTy&...), CapTy&&... capture) {
		Node* current = first;
		while (current) {
			f(current->value, static_cast<CapTy&>(capture)...);
			current = current->next;
		}
	}

	List& operator=(List&& r) noexcept = delete;  /// 代入未実装
};