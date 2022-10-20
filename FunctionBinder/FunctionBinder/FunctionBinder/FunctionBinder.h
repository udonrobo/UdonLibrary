/// @file   FunctionBinder.h
/// @date   2022/10/21
/// @author 大河 祐介

#pragma once

/// @brief 簡易線形リスト
template<class T>
struct LinearList {
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
	void push_back(const T& r) {
		if (tail)  /// 要素有り
			tail = tail->next = new Node{ r };
		else
			head = tail       = new Node{ r };
	}

	struct Iterator {
		Node* p;
		T& operator*() { return p->value; }
		Iterator& operator++() { p = p->next; return *this; }
		bool operator!=(const Iterator& r) { return p != r.p; }
	};
	Iterator begin() { return { head }; }
	Iterator end() {
		if (tail)
			return { tail->next };
		else
			return { head };
	}
};


template<class> class FunctionBinder;

template<class R, class... Args>
class FunctionBinder<R(Args...)> {
	static LinearList<FunctionBinder*> pList;
public:
	static R bind(Args... args) {
		for (const auto& p : pList)  /// 全てのthisポインタ取得
			p->callback(args...);
		return {};
	}
protected:
	FunctionBinder() {
		pList.push_back(this);
	}
	virtual R callback(Args...) = 0;
};
template<class R, class... Args>
LinearList<FunctionBinder<R(Args...)>*> FunctionBinder<R(Args...)>::pList;


/// @brief 部分特殊化 戻り値 : void
template<class... Args>
class FunctionBinder<void(Args...)> {
	static LinearList<FunctionBinder*> pList;
public:
	static void bind(Args... args) {
		for (const auto& p : pList)
			p->callback(args...);
	}
protected:
	FunctionBinder() {
		pList.push_back(this);
	}
	virtual void callback(Args...) = 0;
};
template<class... Args>
LinearList<FunctionBinder<void(Args...)>*> FunctionBinder<void(Args...)>::pList;