#pragma once

namespace container {


/// @brief 簡易双方向線形リスト
/// @remake グローバル領域でのアロケーションが可能です
template<class Ty>
class list {
		struct Node {
			Ty value;
			Node* prev;
			Node* next;
		};
		Node*  _head;
		Node*  _tail;
		size_t _size;

	public:

		list()
			: _head()
			, _tail()
			, _size()
		{}

		~list()
		{
			for (Node* p = _head; p; )
			{
				auto temp = p->next;
				delete p;
				p = temp;
			}
		}

		size_t size() const
		{
			return _size;
		}

		/// @brief 要素を追加する
		/// @param {r} 追加する要素
		void push_back(const Ty& r)
		{
			if (_tail)
			{
				_tail = _tail->next = new Node{ r, _tail, nullptr };
			}
			else
			{
				_tail = _head = new Node{ r, _head, nullptr };
			}
			++_size;
		}

		/// @brief イテレーター
		struct Iterator
		{
			Node* p;
			Ty& operator* () { return  p->value; }
			Ty* operator->() { return &p->value; }
			Iterator& operator++() { p = p->next; return *this; }
			Iterator& operator--() { p = p->prev; return *this; }
			bool operator!=(Iterator r) const { return p != r.p; }
		};
		Iterator begin()
		{
			return { _head };
		}
		Iterator end()
		{
			return { _tail ? _tail->next : _tail };
		}

		/// @brief 要素を削除する
		/// @param {position} 削除する要素のイテレーター
		/// @return 次の要素のイテレーター
		Iterator erase(Iterator position) {
			if (auto pos = position.p)
			{
				// ノードの繋ぎ変えを行う
				if (pos == _tail)
				{
					_tail = pos->prev;
				}
				else
				{
					pos->next->prev = pos->prev;
				}

				if (pos == _head)
				{
					_head = pos->next;
				}
				else
				{
					pos->prev->next = pos->next;
				}
				auto next = pos->next;
				delete pos;
				--_size;
				return { next };
			}
			else
			{
				return end();
			}
		}
};

}
