/// @brief 簡易単方向線形リスト

template<class Ty>
class ForwardLinearList {
		struct Node {
			Ty value;
			Node* next;
		};
		Node* head;
		Node* tail;
	public:
		ForwardLinearList()
			: head()
			, tail()
		{}
		
		~ForwardLinearList() {
			for (Node* it = head; it;) {
				auto temp = it->next;
				delete it;
				it = temp;
			}
		}
	
		/// @brief 要素追加
		/// @param r 追加する要素
		void push_back(const Ty& r) {
			if (tail)
				tail = tail->next = new Node { r };
			else
				head = tail = new Node { r };
		}
	
		/// @brief 要素走査用イテレータ
		struct Iterator {
			Node* p;
			Ty& operator*() { return p->value; }
			Iterator& operator++() { p = p->next; return *this; }
			bool operator!=(const Iterator& r) { return p != r.p; }
		};
		Iterator begin() { return { head }; }
		Iterator end  () { return tail ? Iterator{ tail->next } : Iterator{ head }; }
};
