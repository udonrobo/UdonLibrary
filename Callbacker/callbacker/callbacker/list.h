#pragma once

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
	template<class... CapTy>
	inline void foreach(void (*f)(Ty&, CapTy&...), CapTy&&... capture) {
		Node* current = first;
		while (current) {
			f(current->value, capture...);
			current = current->next;
		}
	}

	List& operator=(List&& r) noexcept = delete;  /// 代入未実装
};