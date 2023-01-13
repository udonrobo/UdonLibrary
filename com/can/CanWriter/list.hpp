#pragma once

namespace container {
	
/// @brief 簡易線形リスト
template<class T>
struct list {
	struct Node {
		T     value;
		Node* next;
	};
	Node* head = nullptr;
	Node* tail = nullptr;

	~LinearList() {
		for (Node* it = head; it;) {
			auto temp = it->next;
			delete it;
			it = temp;
		}
	}

	/// @brief 要素を追加する
	void push_back(const T& r) {
		if (tail) {
			tail = tail->next = new Node;
			tail->value = r;
		}
		else {
			head = tail = new Node;
			head->value = r;
		}
	}

	/// @brief 要素走査用
	struct Iterator {
		Node* p;
		T& operator*() {
			return p->value;
		}
		Iterator& operator++() {
			p = p->next;
			return *this;
		}
		bool operator!=(const Iterator& r) {
			return p != r.p;
		}
	};
	Iterator begin() {
		return { head };
	}
	Iterator end  () {
		return tail ? Iterator{ tail->next } : Iterator{ head };
	}
};

}
