#pragma once

/// @brief 動的配列クラス
template<class T>
class Vector {
	T* buf;
	size_t length;
public:
	Vector() noexcept : buf(), length() {}
	~Vector() noexcept { delete[] buf; }
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


template<class> class FunctionBinder;

/// @brief 
template<class R, class... Args>
class FunctionBinder<R(Args...)> {
	static Vector<FunctionBinder*> pList;
public:
	FunctionBinder() { pList << this; }
	static R bind(Args... args) {
		for (const auto& p : pList)
			p->callback(args...);
	}
	virtual R callback(Args...) = 0;
};
template<class R, class... Args>
Vector<FunctionBinder<R(Args...)>*> FunctionBinder<R(Args...)>::pList;


/// @brief 特殊化:戻り値void
template<class... Args>
class FunctionBinder<void(Args...)> {
	static Vector<FunctionBinder*> pList;
public:
	FunctionBinder() { pList << this; }
	static void bind(Args... args) {
		for (const auto& p : pList)
			p->callback(args...);
	}
	virtual void callback(Args...) = 0;
};
template<class... Args>
Vector<FunctionBinder<void(Args...)>*> FunctionBinder<void(Args...)>::pList;
