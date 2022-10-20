/// @file   CanBase.h
/// @date   2022/09/27
/// @brief  CAN通信基底クラス
/// @author 大河 祐介

#pragma once
template<class T>
class Vector {
public:
	T* buf;
	size_t length;
	Vector() noexcept : buf(), length() {}
	~Vector() noexcept {
		delete[] buf;
	}
	void append(const T& value) {
		++length;
		T* newBuf = new T[length];
		memcpy(newBuf, buf, (length - 1) * sizeof(T));
		delete[] buf;
		buf = newBuf;
		buf[length - 1] = value;
	}
	struct Iterator {
		T* p;
		T& operator*() {
			return *p;
		}
		Iterator& operator++() {
			p++;
			return *this;
		}
		bool operator!=(const Iterator& r) {
			return p != r.p;
		}
	};
	Iterator begin() {
		return { buf };
	}
	Iterator end() {
		return { buf + length };
	}
};


template<class> class FunctionBinder;

template<class R, class... Args>
class FunctionBinder<R(Args...)> {
	static Vector<FunctionBinder*> pList;
public:
	static R bind(Args... args) {
		for (const auto& p : pList)
			p->callback(args...);
		return {};
	}
protected:
	FunctionBinder() { pList << this; }
	virtual R callback(Args...) = 0;
};
template<class R, class... Args>
Vector<FunctionBinder<R(Args...)>*> FunctionBinder<R(Args...)>::pList;


/// @brief 部分特殊化 : 戻り値void
template<class... Args>
class FunctionBinder<void(Args...)> {
	static Vector<FunctionBinder*> pList;
public:
	static void bind(Args... args) {
		for (const auto& p : pList)
			p->callback(args...);
	}
protected:
	FunctionBinder() { pList.append(this); }
	virtual void callback(Args...) = 0;
};
template<class... Args>
Vector<FunctionBinder<void(Args...)>*> FunctionBinder<void(Args...)>::pList;