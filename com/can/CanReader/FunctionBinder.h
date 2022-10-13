/// @file   CanBase.h
/// @date   2022/09/27
/// @brief  CAN通信基底クラス
/// @author 大河 祐介

#pragma once
template<class T>
class Vector {
		T* buf;
		size_t length;
	public:
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
		void update() {

			Serial.println(length);
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
<<<<<<< .merge_file_a39292
//
//template<class R, class... Args>
//class FunctionBinder<R(Args...)> {
//	static Vector<FunctionBinder*> pList;
//public:
//	FunctionBinder() { pList.append(this); }
//	static R bind(Args... args) {
//		for (const auto& p : pList)
//			p->callback(args...);
//		return {};
//	}
//	virtual R callback(Args...) = 0;
//};
//template<class R, class... Args>
//Vector<FunctionBinder<R(Args...)>*> FunctionBinder<R(Args...)>::pList;


/// @brief 特殊化:戻り値void
=======

/// @brief 特殊化 : 戻り値void
>>>>>>> .merge_file_a36720
template<class... Args>
class FunctionBinder<void(Args...)> {
		static Vector<FunctionBinder*> pList;
	public:
		FunctionBinder() {
			pList.append(this);
			Serial.println("call");
			pList.update();
			pList.update();
			pList.update();
			pList.update();
		}
		static void bind(Args... args) {
<<<<<<< .merge_file_a39292
			pList.update();
			//		for (const auto& p : pList){
			//			p->callback(args...);
			//		}
=======
			for (const auto& p : pList)
				p->callback(args...);
>>>>>>> .merge_file_a36720
		}
		virtual void callback(Args...) = 0;
};
template<class... Args>
Vector<FunctionBinder<void(Args...)>*> FunctionBinder<void(Args...)>::pList;
