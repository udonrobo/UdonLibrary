/// @file   Callbacker.h
/// @date   2022/09/26
/// @brief  クラスのメンバ関数を静的関数にバインドするクラス
/// @author 大河 祐介

/// 1つの関数のみ登録するモード(処理ちょっと軽い)
//#define SINGLE_CALLBACK

#include "list.h"

//template<class... PTy>
//class _Callbacker {
//
//	static List<_Callbacker*> pList;
//
//public:
//
//	/// @brief 静的関数
//	static void bind(PTy... param) {
//		pList.foreach<_Callbacker*, PTy...>(accessor, param...);
//	}
//
//	static void accessor(_Callbacker*&& p, PTy&&... param) {  /// なぜかラムダ式にできない
//		p->callback(param);
//	}
//
//protected:
//
//	/// インスタンスを追加
//	_Callbacker() {
//		pList << this;
//	}
//
//	/// @brief コールバックされる純粋仮想関数
//	virtual void callback(PTy...) = 0;
//};
//
//template<class... T>
//List<_Callbacker<T...>*> _Callbacker<T...>::pList;
//
//
//void (*f)(int);
//
//void on(void(*c)(int))
//{
//	f = c;
//}
//void call()
//{
//	f(100);
//}



#include <iostream>
#include <thread>
#include <functional>

//struct Hoge : _Callbacker<int>
//{
//	void callback(int val) override
//	{
//		std::cout << val << "call\n";
//	}
//};

void call(int& val, int& a, int& b)
{
	std::cout << val << a << b << std::endl;;
}

int main()
{
	//Hoge as[5] = {};
	//on(_Callbacker<int>::bind);
	//call();
	List<int> list;
	list << 1233 << 12313 << 1233;

	int a = 0;

	list.foreach(call, static_cast<int&&>(a), 200);
}