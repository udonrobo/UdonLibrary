#include <iostream>
#include "FunctionBinder.h"
 

struct Massege_t {
	int data;
};

using FunctionBinder_t = FunctionBinder<void(Massege_t)>;

struct Reader : private FunctionBinder_t {

	void callback(Massege_t arg) override {
		std::cout << arg.data << std::endl;
	}
};

Reader reader;
Reader reader1;
Reader reader2;

int main() {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);  /// メモリリーク検出用


	FunctionBinder_t::bind({ 100 });
}