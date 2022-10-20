#include <iostream>
#include "Reader.h"


template<class T>
struct list {
	struct Node {
		T     value;
		Node* next;
	};
	Node* head = nullptr;
	Node* tail = nullptr;

	~list() {
		for (Node* it = head; it;) {
			auto temp = it->next;
			delete it;
			it = temp;
		}
	}
	void push_back(const T& r)
	{
		if (tail)
			tail = tail->next = new Node{ r };
		else  /// 要素無し
			head = tail = new Node{ r };
	}

	struct Iterator {
		Node* p;
		T& operator*() { return p->value; }
		Iterator& operator++() { p = p->next; return *this; }
		bool operator!=(const Iterator& r) { return p != r.p; }
	};
	Iterator begin() { return { head }; }
	Iterator end()   {
		if (tail)
			return { tail->next };
		else
			return { head };
	}
};

Reader reader;

int main() {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	list<int> l;

	l.push_back(100);
	l.push_back(101);
	l.push_back(102);
	l.push_back(103);
	l.push_back(104);
	for (auto& it : l)
		std::cout << it << std::endl;

	//std::cout << "call" << std::endl;
	//FunctionBinder<void(Massege_t)>::bind({ 100 });
	//std::cout << "end" << std::endl;
}