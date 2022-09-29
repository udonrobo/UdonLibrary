#pragma once

template<class> class FunctionBinder;

template<class R, class... Args>
class FunctionBinder<R(Args...)> {

	/// @brief 動的配列クラス
	template<class T>
	class Vector {
		T* buf;
		size_t length;
	public:
		Vector() noexcept : buf(), length() {}
		~Vector() noexcept { delete[] buf; }

		/// @brief 要素追加
		Vector& operator<<(const T& value) {
			++length;
			T* newBuf = new T[length];
			memcpy(newBuf, buf, (length - 1) * sizeof(T));
			delete[] buf;
			buf = newBuf;
			buf[length - 1] = value;
			return *this;
		}
		struct Iterator {
			T* p;
			T& operator*() { return *p; }
			Iterator& operator++() { p++; return *this; }
			bool operator!=(const Iterator& r) { return p != r.p; }
		};
		Iterator begin() { return { buf }; }
		Iterator end() { return { buf + length }; }
	};
public:
	static Vector<FunctionBinder*> pList;

	FunctionBinder() {
		pList << this;
	}
	static R bind(Args... args) {
		for (const auto& p : pList) {
			p->callback(args...);
		}
	}
	virtual R callback(Args...) = 0;
};

template<class R, class... Args>
FunctionBinder<R(Args...)>::Vector<FunctionBinder<R(Args...)>*> FunctionBinder<R(Args...)>::pList;
