/// @file   Callbacker.h
/// @date   2022/09/26
/// @brief  クラスのメンバ関数を静的関数にバインドするクラス
/// @author 大河 祐介

#include "list.h"

template<class... PTy>
class FunctionBinder {

	static List<FunctionBinder*> pList;

	static void accessor(FunctionBinder*& p, PTy&... param) {  /// なぜかラムダ式にできない
		p->callback(param...);
	}

public:

	/// @brief 静的関数
	static void bind(PTy... param) {
		pList.foreach(accessor, static_cast<PTy&&>(param)...);
	}

	using Parameter = PTy;

protected:

	/// @brief インスタンスを追加
	FunctionBinder() {
		pList << this;
	}

	/// @brief コールバックされる純粋仮想関数
	virtual void callback(PTy&...) = 0;
};

template<class... T>
List<FunctionBinder<T...>*> FunctionBinder<T...>::pList;


void (*f)(int, int);

void on(void(*c)(int, int))
{
	f = c;
}
void call()
{
	f(100, 200);
}

#include <iostream>

struct Hoge : FunctionBinder<int, int> {
	void callback(int& val, int& val2) override
	{
		std::cout << val << val2 <<  "call\n";
	}
};


int main()
{
	Hoge as[5] = {};
	on(FunctionBinder<int ,int>::bind);
	call();
	call();
}